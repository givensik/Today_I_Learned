#include <stdio.h>
#include <stdlib.h>

/**
 * DOS DateTime 유효성 검사 함수
 * @return 0: 정상(NORMAL), 1: 에러(ERROR)
 */
int check_invalid(int y, int m, int d, int h, int min, int s) {
    // 1. 기본 범위 검사
    if (m < 1 || m > 12) return 1;
    if (h < 0 || h > 23) return 1;
    if (min < 0 || min > 59) return 1;
    if (s < 0 || s > 59) return 1;

    // 2. 월별 말일 계산 (윤년 포함)
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // 윤년 판정: 4로 나누어 떨어지고 100으로 나누어 떨어지지 않거나, 400으로 나누어 떨어지는 해
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
        days_in_month[2] = 29;
    }

    // 3. 해당 월의 일자 범위 검사
    if (d < 1 || d > days_in_month[m]) return 1;

    return 0;
}

int main() {
    FILE *fp_in = fopen("dos_time_set.csv", "r");
    FILE *fp_out = fopen("dos_time_checked.csv", "w");

    if (fp_in == NULL || fp_out == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    char buffer[256];
    unsigned int dos_val;

    // 결과 파일 헤더 작성
    fprintf(fp_out, "Hex,DateTime,Status\n");

    // CSV 파일 읽기 (한 줄씩 읽어서 16진수 추출)
    while (fgets(buffer, sizeof(buffer), fp_in)) {
        // fscanf 대신 fgets + sscanf를 사용하면 헤더나 빈 줄 처리에 유연합니다.
        if (sscanf(buffer, "%x", &dos_val) != 1) continue;

        // --- 비트 추출 로직 (DOS DateTime 32bit 구조) ---
        // 상위 16비트: Date (Year 7bit, Month 4bit, Day 5bit)
        // 하위 16비트: Time (Hour 5bit, Minute 6bit, Second 5bit)
        
        int year   = ((dos_val >> 25) & 0x7F) + 1980; // 0=1980
        int month  = (dos_val >> 21) & 0x0F;
        int day    = (dos_val >> 16) & 0x1F;
        
        int hour   = (dos_val >> 11) & 0x1F;
        int minute = (dos_val >> 5)  & 0x3F;
        int second = (dos_val & 0x1F) * 2; // DOS는 2초 단위로 저장

        // 유효성 검사
        int is_error = check_invalid(year, month, day, hour, minute, second);

        // 출력 및 파일 저장
        if (is_error) {
            fprintf(fp_out, "0x%08X,%04d-%02d-%02d %02d:%02d:%02d,ERROR\n", 
                    dos_val, year, month, day, hour, minute, second);
            printf("0x%08X -> ERROR (Invalid Date/Time)\n", dos_val);
        } else {
            fprintf(fp_out, "0x%08X,%04d-%02d-%02d %02d:%02d:%02d,OK\n", 
                    dos_val, year, month, day, hour, minute, second);
            printf("0x%08X -> %04d-%02d-%02d %02d:%02d:%02d (OK)\n", 
                   dos_val, year, month, day, hour, minute, second);
        }
    }

    printf("\n처리가 완료되었습니다. 'dos_time_checked.csv'를 확인하세요.\n");

    fclose(fp_in);
    fclose(fp_out);

    return 0;
}