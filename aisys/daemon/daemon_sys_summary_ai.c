#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <time.h>
#include <sys/sysinfo.h>  // 메모리/uptime 정보용
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ai_helper.c"  // AI 요약 기능 (libcurl 사용)

// AI 프롬프트 템플릿
#define PROMPT_TEMPLATE_SUMMARY_SYSTEM "You are a helpful assistant that summarizes system information concisely."
#define PROMPT_TEMPLATE_SUMMARY_USER "Analyze this system data and rate it: Very Good, Good, Moderate, Poor, or Bad. Answer in one short sentence."

// 함수 원형
void collect_system_info(char *buffer, size_t buffer_size);
void daemonize(const char *cmd);

// ----- 메인 함수 -----
// 5초마다 시스템 정보를 수집하여 AI로 요약하고 /tmp/sysstat.log에 기록
int main(void) {

    char system_info[16384];   // 시스템 정보 수집 버퍼
    char full_prompt[32768];   // AI 프롬프트 (질문 + 시스템 정보)
    char ai_response[32768];   // AI 응답 버퍼

    printf("daemon pid : %d\n", getpid());
    daemonize("sysstatd");  // 데몬화
        
    FILE *fp;
    struct sysinfo info;

    while (1) {
        if (sysinfo(&info) == 0) {
            fp = fopen("/tmp/sysstat.log", "a");
            if (fp) {
                time_t now = time(NULL);
                fprintf(fp, "=== %s", ctime(&now));
                
                // 시스템 정보 수집
                collect_system_info(system_info, sizeof(system_info));
                
                // 프롬프트와 시스템 정보 결합
                full_prompt[0] = '\0';
                snprintf(full_prompt, sizeof(full_prompt), "%s\n\n%s",
                         PROMPT_TEMPLATE_SUMMARY_USER, system_info);

                fprintf(fp, "[DEBUG] Full prompt : %s\n", full_prompt);

                // AI에게 요약 요청 (ollama API 호출)
                get_ai_summary(full_prompt, ai_response, sizeof(ai_response));
                fprintf(fp, "[AI Summary] %s\n", ai_response);
                fclose(fp);
            }
        }
        sleep(5);  // 5초마다 실행
    }
}

// 프로세스를 데몬으로 변환하는 함수
void daemonize(const char *cmd)
{   
	int					i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit		rl;
	struct sigaction	sa;

	// 파일 생성 마스크 초기화
	umask(0);

	// 파일 디스크립터 최대 개수 얻기
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: can't get file limit", cmd);

	// 1차 fork: 세션 리더가 되어 터미널에서 분리
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0)
		exit(0);
	setsid();

	// SIGHUP 무시 및 2차 fork: 터미널 재할당 방지
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: can't ignore SIGHUP", cmd);
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0)
		exit(0);

	// 작업 디렉토리를 루트로 변경
	if (chdir("/") < 0)
		err_quit("%s: can't change directory to /", cmd);

	// 모든 파일 디스크립터 닫기
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	// stdin, stdout, stderr를 /dev/null로 재연결
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	// syslog 초기화 (데몬은 터미널이 없으므로 로그를 시스템에 기록)
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
		  fd0, fd1, fd2);
		exit(1);
	}
}
// ----- 시스템 정보 수집 함수 -----
// 다양한 시스템 메트릭을 수집하여 버퍼에 저장
void collect_system_info(char *buffer, size_t buffer_size)
{
    struct sysinfo info;
    buffer[0] = '\0';

    if (sysinfo(&info) != 0) return;
    
    char line[1024];
    
    // 1. Uptime 및 Load Average 정보
    long uptime_hours = info.uptime / 3600;
    long uptime_mins = (info.uptime % 3600) / 60;
    snprintf(line, sizeof(line), 
             "Uptime: %ldh %ldm, Load: %.2f/%.2f/%.2f\n",
             uptime_hours, uptime_mins,
             info.loads[0] / 65536.0,   // 1분 평균
             info.loads[1] / 65536.0,   // 5분 평균
             info.loads[2] / 65536.0);  // 15분 평균
    strncat(buffer, line, buffer_size - strlen(buffer) - 1);
    
    // 2. 메모리 사용 정보
    unsigned long total_ram = info.totalram / 1024 / 1024;
    unsigned long free_ram = info.freeram / 1024 / 1024;
    unsigned long used_ram = total_ram - free_ram;
    unsigned long buff_ram = info.bufferram / 1024 / 1024;
    unsigned long shared_ram = info.sharedram / 1024 / 1024;
    
    snprintf(line, sizeof(line),
             "Memory: %luMB used / %luMB total (%.1f%%), %luMB free, %luMB buffer, %luMB shared\n",
             used_ram, total_ram, (used_ram * 100.0 / total_ram),
             free_ram, buff_ram, shared_ram);
    strncat(buffer, line, buffer_size - strlen(buffer) - 1);
    
    // 3. Swap 메모리 정보
    unsigned long total_swap = info.totalswap / 1024 / 1024;
    unsigned long free_swap = info.freeswap / 1024 / 1024;
    unsigned long used_swap = total_swap - free_swap;
    
    snprintf(line, sizeof(line),
             "Swap: %luMB used / %luMB total, %luMB free\n",
             used_swap, total_swap, free_swap);
    strncat(buffer, line, buffer_size - strlen(buffer) - 1);
    
    // 4. 실행 중인 프로세스 수
    snprintf(line, sizeof(line), "Processes: %d\n", info.procs);
    strncat(buffer, line, buffer_size - strlen(buffer) - 1);
    
    // 5. CPU 코어 개수
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    snprintf(line, sizeof(line), "CPU cores: %ld\n", num_cpus);
    strncat(buffer, line, buffer_size - strlen(buffer) - 1);
    
    // 6. /proc/stat에서 CPU 통계 읽기
    FILE *fp = fopen("/proc/stat", "r");
    if (fp) {
        char cpu_line[256];
        if (fgets(cpu_line, sizeof(cpu_line), fp)) {
            strncat(buffer, "CPU: ", buffer_size - strlen(buffer) - 1);
            strncat(buffer, cpu_line, buffer_size - strlen(buffer) - 1);
        }
        fclose(fp);
    }
    
    // 7. /proc/meminfo에서 캐시 및 사용 가능한 메모리 정보
    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        char mem_line[256];
        int cached = 0, available = 0;
        while (fgets(mem_line, sizeof(mem_line), fp)) {
            if (strncmp(mem_line, "Cached:", 7) == 0) {
                sscanf(mem_line + 7, "%d", &cached);
            } else if (strncmp(mem_line, "MemAvailable:", 13) == 0) {
                sscanf(mem_line + 13, "%d", &available);
            }
        }
        snprintf(line, sizeof(line), "Cached: %dMB, Available: %dMB\n", 
                 cached / 1024, available / 1024);
        strncat(buffer, line, buffer_size - strlen(buffer) - 1);
        fclose(fp);
    }
    
    // 8. /proc/loadavg에서 상세한 부하 정보 (실행/전체 프로세스)
    fp = fopen("/proc/loadavg", "r");
    if (fp) {
        char loadavg_line[256];
        if (fgets(loadavg_line, sizeof(loadavg_line), fp)) {
            strncat(buffer, "LoadAvg detail: ", buffer_size - strlen(buffer) - 1);
            strncat(buffer, loadavg_line, buffer_size - strlen(buffer) - 1);
        }
        fclose(fp);
    }
}
