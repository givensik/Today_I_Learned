// mini-shell_socket.c

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MY_PORT 5555

int main(void) {

    // ---------- 1) TCP 소켓 생성 ----------
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0) {
        perror("socket");
        return 1;
    }

    // ---------- 2) 서버 주소 설정 ----------
    // 소켓 주소 구조체(ipv4)
    struct sockaddr_in serv_addr;
    // memset 을 이용한 구조체 초기화 
    // your code here
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 주소 체계: IPv4 설정
    // your code here
    serv_addr.sin_family = AF_INET;
    // 포트 번호 설정
    serv_addr.sin_port = htons(MY_PORT);
    // your code here
    // IP 주소 설정 : 127.0.0.1, string 형태의 주소를 binary 형태로 변환         
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // ---------- 3) 서버 접속 ----------
    // connect : 서버에 접속 요청
    if (connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // ---------- 4) 기존 파이프 버전 mini-shell 로직 그대로 ----------
    char buf[1024];

    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin))
            break;

        if (strncmp(buf, "exit", 4) == 0)
            break;

        // 서버로 요청 전송 (이전: write(to_child[1]...) )
        // your code here
        write(cfd, buf, strlen(buf));
        write(STDOUT_FILENO, "[AI Response] ", 14);
        // ---------- 스트리밍 파서 (그대로 유지) ----------
        const char *end_marker = "<<<END>>>";
        size_t marker_len = strlen(end_marker);

        char acc[4096];
        size_t acc_len = 0;

        ssize_t nread;
    
        while (1) {
            
            // 서버 스트리밍 수신, 그냥 read() 하면 됨. 
            // your code here
            nread = read(cfd, buf, sizeof(buf)-1);
            if (nread <= 0)
                break;

            // // 디버그: 읽은 바이트 수와 원시 데이터를 stderr에 출력
            // buf[nread] = '\0';
            // fprintf(stderr, "[DEBUG] read %zd bytes:\n", nread);
            // // 쓰레드/비차단 환경에서 안전하게 출력
            // fwrite(buf, 1, nread, stderr);
            // fprintf(stderr, "\n[DEBUG] end\n");

            // acc 버퍼 overflow 방지 + memmove 유지
            if ((size_t)nread + acc_len >= sizeof(acc) - 1) {
                size_t keep = marker_len ? marker_len - 1 : 0; // 마커 길이만큼은 남겨둠
                size_t flush = acc_len > keep ? acc_len - keep : 0; // 나머지 부분은 출력
                if (flush > 0) {
                    write(STDOUT_FILENO, acc, flush);
                    memmove(acc, acc + flush, acc_len - flush);
                    acc_len -= flush;
                }
            }

            // acc에 데이터 누적
            memcpy(acc + acc_len, buf, nread);
            acc_len += nread;
            acc[acc_len] = '\0';

            // END 마커 검색
            char *pos = NULL;
            if (acc_len >= marker_len)
                pos = strstr(acc, end_marker); // 마커 위치 검색

            if (pos) { // 마커 발견
                size_t to_write = pos - acc;
                if (to_write > 0)
                    write(STDOUT_FILENO, acc, to_write);

                size_t remain = acc_len - (to_write + marker_len);
                if (remain > 0)
                    memmove(acc, pos + marker_len, remain);

                acc_len = remain;
                acc[acc_len] = '\0';
                break;
            } else {
                // 버퍼 커지면 앞부분 미리 출력
                if (acc_len > marker_len + 64) {
                    size_t safe = acc_len - (marker_len + 64);
                    write(STDOUT_FILENO, acc, safe);
                    memmove(acc, acc + safe, acc_len - safe);
                    acc_len -= safe;
                }
            }
        }

        if (acc_len > 0)
            write(STDOUT_FILENO, acc, acc_len);

        write(STDOUT_FILENO, "\n", 1);
    }

    // ---------- 5) 소켓 종료 ----------
    // your code here
    close(cfd);
    return 0;
}
