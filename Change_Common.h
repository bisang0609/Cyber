/**
 * @file Change_Common.h
 * @brief 프로젝트 공통 보안 강화 매크로 및 유틸리티
 * @note 본 파일은 사이버 보안 가이드라인(CWE-120, CWE-119)에 따라 
 * 버퍼 오버플로우 방지를 위한 안전한 문자열 처리 로직을 포함함.
 */

#ifndef __CHANGE_COMMON_H__
#define __CHANGE_COMMON_H__

#include <string.h>

/* -------------------------------------------------------------------------- */
/* [보안 강화 매크로: SAFE_STRCPY]                                             */
/* -------------------------------------------------------------------------- */
/**
 * @brief 고정 크기 배열을 위한 안전한 strcpy 대체 매크로
 * @param dest 복사 대상 (반드시 '배열'이어야 함)
 * @param src  복사 원본
 * @warning 포인터 변수(char*)에는 사용 금지 (sizeof 연산 오류 발생)
 */
#define SAFE_STRCPY(dest, src) do { \
    if ((void*)(dest) != NULL && (void*)(src) != NULL) { \
        strncpy((char*)(dest), (const char*)(src), sizeof(dest) - 1U); \
        (dest)[sizeof(dest) - 1U] = '\0'; \
    } \
} while(0)

/* -------------------------------------------------------------------------- */
/* [보안 강화 매크로: SAFE_STRCPY_PTR]                                         */
/* -------------------------------------------------------------------------- */
/**
 * @brief 포인터(char*)를 위한 안전한 strcpy 대체 매크로
 * @param dest     복사 대상 포인터
 * @param src      복사 원본
 * @param destSize 대상 버퍼의 실제 할당 크기
 */
#define SAFE_STRCPY_PTR(dest, src, destSize) do { \
    if ((void*)(dest) != NULL && (void*)(src) != NULL && (destSize) > 0U) { \
        strncpy((char*)(dest), (const char*)(src), (size_t)(destSize) - 1U); \
        (dest)[(size_t)(destSize) - 1U] = '\0'; \
    } \
} while(0)

#endif /* __CHANGE_COMMON_H__ */
