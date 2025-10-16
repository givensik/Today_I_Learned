#include <unistd.h>
#include <string.h>
#include <stdio.h>
// #include <crypt.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <setjmp.h>

#define FILENAME "mypasswd"
#define SALT_CHARS "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define SALT_LEN 12   /* 실제 솔트 길이 (문자수). shadow 스타일의 $id$salt$ 형식에서 적당한 길이 선택 */
#define SALT_BUF 64
#define LINE_BUF 512

typedef struct {
    char name[32];
    char hash[256];
} USER;

/* 안전한 랜덤 솔트 생성: /dev/urandom에서 바이트 읽어 SALT_CHARS 인덱스로 변환 */
char *make_salt(void) {
    // your code here
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd < 0) {
        perror("open /dev/urandom");
        return NULL;
    }
    unsigned char rnd[SALT_LEN];
    ssize_t r = read(fd, rnd, sizeof(rnd));
    close(fd);
    if (r != sizeof(rnd)) {
        fprintf(stderr, "could not read enough random bytes\n");
        return NULL;
    }

    char *salt = malloc(SALT_BUF);
    if (!salt) return NULL;

    /* 여기서는 MD5($1$) 대신 SHA-512($6$) 또는 yescrypt($y$) 등을 쓸 수 있음.
       실습 편의를 위해 SHA-512($6$)로 생성. 원하는 알고리즘으로 바꾸십시오. */
    /* 형식: $6$<salt>$   (SHA-512) */
    const char *prefix = "$6$";
    size_t pos = 0;
    strcpy(salt, prefix);
    pos = strlen(prefix);
    for (int i = 0; i < SALT_LEN && pos + 1 < SALT_BUF - 1; ++i) {
        salt[pos++] = SALT_CHARS[rnd[i] % (sizeof(SALT_CHARS)-1)];
    }
    salt[pos++] = '$';
    salt[pos] = '\0';
    return salt;
}

char *make_hash(const char *password, const char *salt) {
    // 결과를 malloc 해서 복사하고 반환할 수도 있으나 
    // strdup로 복사해서 반환하는 것이 편리(
    // strdup() = malloc(strlen(s)+1),strcpy() 
    char *r = crypt(password, salt);
    if (!r) {
        perror("crypt"); // perror은 에러를 출력하는 함수
        return NULL;
    }
    return strdup(r);
}

void save_user(const char *name, const char *password) {
    char *salt = make_salt();
    int fd;
    if (!salt) {
        fprintf(stderr, "salt generation failed\n");
        return;
    }
    char *hash = make_hash(password, salt);
    free(salt); /* 솔트는 더 이상 필요 없음 */
    if (!hash) return;

    /* 파일을 append 모드로 0600 권한으로 열기 */
    // your code here
    fd = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, 0600);
    if (fd == -1) {
        perror("open");
        free(hash);
        return;
    }

    char line[LINE_BUF];
    int n = snprintf(line, sizeof(line), "%s:%s\n", name, hash);
    if (n < 0 || n >= (int)sizeof(line)) {
        fprintf(stderr, "line too long\n");
        close(fd);
        free(hash);
        return;
    }
    ssize_t w = write(fd, line, strlen(line));
    if (w < 0) perror("write");
    fsync(fd); // 디스크에 확실히 기록
    close(fd);

    printf("User %s saved (hash %.20s...)\n", name, hash);
    free(hash); // 이제 기록했으니 필요 없음. 
}

/* mypasswd 파일에서 유저 검색 */
USER *getusrent(const char *name) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        /* 없으면 NULL 반환(파일 없음) */
        return NULL;
    }

    static USER user;
    char line[LINE_BUF];

    while (fgets(line, sizeof(line), fp)) {
        char *p = strchr(line, ':');
        if (!p) continue;
        *p = '\0';
        char *hash_part = p + 1;
        char *nl = strchr(hash_part, '\n');
        if (nl) *nl = '\0';

        if (strcmp(line, name) == 0) {
            strncpy(user.name, line, sizeof(user.name)-1);
            user.name[sizeof(user.name)-1] = '\0';
            strncpy(user.hash, hash_part, sizeof(user.hash)-1);
            user.hash[sizeof(user.hash)-1] = '\0';
            fclose(fp);
            return &user;
        }
    }

    fclose(fp);
    return NULL;
}

jmp_buf env;
int login_attempts = 0;

int main(void) {
    char name[32];
    char *password;

    int ret = setjmp(env);
    if (ret != 0) {
        // longjmp로 돌아온 경우
        if (login_attempts >= 3) {
            printf("Too many failed attempts. Exiting.\n");
            return 1;
        }
        printf("Retry login... (attempt %d/3)\n", login_attempts+1);
    }

    printf("Username: ");
    if (scanf("%31s", name) != 1) {
        fprintf(stderr, "input error\n");
        return 1;
    }
    // 남은 입력 버퍼 비우기
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

    // getpass로 에코 없이 비밀번호 입력
    password = getpass("Password: ");
    if (!password) {
        fprintf(stderr, "getpass failed\n");
        return 1;
    }

    USER *user = getusrent(name);
    if (!user) { // 신규 사용자 생성
        printf("No such user. Creating new user '%s'.\n", name);
        save_user(name, password);
        printf("User %s created. Re-run to login.\n", name);

        // 다시 처음으로 돌아감. 수업때 배웠던 longjmp를 사용합니다. 
        // your code here
        longjmp(env, login_attempts=0);
        return 0;
    }

    // 검증: stored hash의 전체 문자열을 crypt의 두번째 인수로 넣어 비교
    char *res = crypt(password, user->hash);
    if (!res) {
        perror("crypt");
        return 1;
    }

    if (strcmp(res, user->hash) == 0) {
        printf("Login successful. Launching mini shell...\n");

        // 환경 변수 설정(실습용)
        // user code here 
        setenv("USER", name, 1);
        // HOME 환경변수를 현재 디렉터리로 설정
        // user code here
        setenv("HOME", ".", 1);
        // login 역할: 인증 완료 후 shell 실행
        // your code here
        execl("./minishell", "minishell", (char *)NULL);
        // execl 실패시 에러 출력
        perror("execl minishell");
        return 1;
    } else {
        printf("Login failed.\n");
        // your code here
        login_attempts++;
        longjmp(env, login_attempts);
        return 1;
    }
}