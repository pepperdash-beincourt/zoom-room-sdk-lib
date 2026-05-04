/*!
 * \file ZRCSDKDefines.h
 * \brief ZRC SDK Defines
 */
#ifndef ZRC_ZRCSDKDEFINES_H
#define ZRC_ZRCSDKDEFINES_H

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#ifdef ZRCSDK_EXPORT
#define ZRCSDK_API __declspec(dllexport)
#else
#define ZRCSDK_API __declspec(dllimport)
#endif

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__) || defined(ANDROID)
#define ZRCSDK_API __attribute__((visibility("default")))

#elif defined(__linux) || defined(__linux__) || defined(linux)
#define ZRCSDK_API __attribute__((visibility("default")))

#else
#define ZRCSDK_API
#endif

#define NS_ZRCSDK ZRCSDK
#define NS_ZRCSDK_BEG \
    namespace ZRCSDK  \
    {
#define NS_ZRCSDK_END   }
#define USING_NS_ZRCSDK using namespace NS_ZRCSDK;

#define ZRCSDK_MEETING_DATE_FORMAT "yyyy-MM-dd'T'HH:mm:ssZZZZZ"

#define ZRCSDK_DEFAULT_ROOM_ID "zrcsdk_default_room_id"

#endif    //ZRC_ZRCSDKDEFINES_H
