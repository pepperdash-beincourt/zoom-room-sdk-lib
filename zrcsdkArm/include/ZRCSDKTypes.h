/*!
 * \file ZRCSDKTypes.h
 * \since Added in ZRC SDK 6.0.0
 * \brief ZRC SDK Types
 */
#ifndef ZRC_ZRCSDKTYPES_H
#define ZRC_ZRCSDKTYPES_H

#include <string>
#include <vector>
#include <map>

#include "ZRCSDKDefines.h"

NS_ZRCSDK_BEG

/*! \enum ZRCSDKError
    \brief SDK error types.
    Here are more detailed structural descriptions.
 */
enum ZRCSDKError
{
    // General Error Code
    ZRCSDKERR_SUCCESS = 0,                       ///<Success.
    ZRCSDKERR_INTERNAL_ERROR = 1,                ///<Internal error.
    ZRCSDKERR_SERVICE_UNINITIALIZE = 2,          ///<Not initialized before the usage.
    ZRCSDKERR_NO_PERMISSION = 3,                 ///<No permission.
    ZRCSDKERR_FEATURE_DISABLED = 4,              ///<Feature is disabled.
    ZRCSDKERR_ZR_NO_CAPABILITY = 5,              ///<Zoom Room has no capability to process the action.
    ZRCSDKERR_INVALID_PARAMETER = 6,             ///<Invalid parameter.
    ZRCSDKERR_EVALUATION_DATE_EXPIRED = 7,       ///<Running an evaluation build which has expired.
    ZRCSDKERR_API_NOT_SUPPORT_IN_WEBINAR = 8,    ///<The API is not supported in webinar meeting.
    ZRCSDKERR_API_WRONG_USAGE = 9,               ///<Wrong usage of API.
    ZRCSDKERR_ALREADY_IN_THIS_STATE = 10,        ///<Already in this state.
    ZRCSDKERR_NOT_CONNECT_TO_ZOOMROOM = 11,      ///<Not connect to Zoom Room.
    ZRCSDKERR_HOST_WITHOUT_CAPABILITY = 12,      ///<Host no capability to response
    ZRCSDKERR_HOST_NOT_IN_MEETING = 13,          ///<Host has not yet joined the meeting
    ZRCSDKERR_CAN_NOT_PERFORM_ACTION = 14,       ///<SDK cannot perform the action because ZR is in special mode. Current special mode includes Supporting Mode.
    ZRCSDKERR_API_NOT_SUPPORT_IN_WEBINAR_BACKSTAGE = 15, ///<The API is not supported in webinar backstage.
    ZRCSDKERR_REQUEST_HAS_BEEN_RESPONSED = 16,           ///<Request has been responsed.
    ZRCSDKERR_WEB_SETTINGS_LOCKED = 17,         ///<Settings is locked on web.

    ZRCSDKERR_API_NOT_SUPPORT_IN_MEETING = 50,                    ///<The API is not supported in normal meeting.
    ZRCSDKERR_API_NOT_SUPPORT_IN_BO = 51,                         ///<The API is not supported in breakout room.
    ZRCSDKERR_API_NOT_SUPPORT_IN_WEBINAR_BO = 52,                 ///<The API is not supported in webinar breakout room.
    ZRCSDKERR_API_NOT_SUPPORT_IN_WEBINAR_DEBRIEF_SESSION = 53,    ///<The API is not supported in webinar debrief session.
    ZRCSDKERR_API_NOT_SUPPORT_IN_SWITCHING_MEETING = 54,          ///<The API is not supported while switching meeting.
    ZRCSDKERR_API_NOT_SUPPORT_IN_SIMULIVE_WEBINAR = 55,           ///<The API is not supported in simulive webinar.

    // Device Error Code
    ZRCSDKERR_DEVICE_NOT_EXIST = 100,               ///<Device not exist.
    ZRCSDKERR_MIC_DISABLED = 101,                   ///<Microphone is disabled.
    ZRCSDKERR_CAMERA_DISABLED = 102,                ///<Camera is disabled.
    ZRCSDKERR_NO_CEC_SCREEN = 103,                  ///<NO CEC screen.
    ZRCSDKERR_AUDIO_IS_MUTED = 104,                 ///<Current microphone's audio is muted.
    ZRCSDKERR_NO_LOCAL_NETWORK_AUDIO_DEVICE = 105,  ///<No local network audio device.
    ZRCSDKERR_ZR_NOT_IOT_DEVICE = 106,              ///<Zoom Room is not a IoT device.
    ZRCSDKERR_DEVICE_NOT_SUPPORT_IDENTIFY = 107,    ///<Device not support identify
    ZRCSDKERR_NETWORK_AUDIO_CHANNEL_NOT_FOUND = 108,///<Network audio device channel not found.
    ZRCSDKERR_DEVICE_NOT_CONNECTED = 109,           ///<Device not connected.
    ZRCSDKERR_SELECTED_MIC_IS_STETHOSCOPE = 110,    ///<The selected microphone is stethoscope.
    ZRCSDKERR_DEVICE_NO_CAPABILITY = 111,           ///<The device has no capability.
    ZRCSDKERR_MIC_IS_TROUBLESHOOTING = 112,         ///<The microphone is troubleshooting.

    ZRCSDKERR_CAMERA_CAN_NOT_CONTROL = 120,                          ///<Camera can't control.
    ZRCSDKERR_CAMERA_CAN_NOT_MOVE = 121,                             ///<Camera can't move.
    ZRCSDKERR_CAMERA_CAN_NOT_ZOOM = 122,                             ///<Camera can't zoom.
    ZRCSDKERR_CAMERA_CAN_NOT_SWITCH = 123,                           ///<Camera can't switch to another one.
    ZRCSDKERR_CAMERA_CAN_NOT_CHANGE_PANTILT_SPEED = 124,             ///<Camera can't change pan tilt speed.
    ZRCSDKERR_CAMERA_NOT_SUPPORT_THE_SMART_MODE = 125,               ///<Camera not support the smart camera mode.
    ZRCSDKERR_CAMERA_NOT_ACTIVE = 126,                               ///<Camera is not active.
    ZRCSDKERR_CAMERA_CONTROL_NOT_ENABLE = 127,                       ///<Camera control is not enable.
    ZRCSDKERR_CAMERA_THIS_CAMERA_IS_NOT_SHARING = 128,               ///<Camera is not sharing.
    ZRCSDKERR_CAMERA_NOT_SUPPORT_PRESET = 129,                       ///<Camera not support preset.
    ZRCSDKERR_CAMERA_NOT_SUPPORT_SPEAKER_SWITCHING = 130,            ///<Not support speaker switching.
    ZRCSDKERR_CAMERA_CAN_NOT_ENABLE_COMPANION_ZR_CAMERA = 131,       ///<Companion ZR's camera can't enable.
    ZRCSDKERR_CAMERA_NOT_SUPPORT_DIRECTOR_CALIBRATE = 132,           ///<Not support director calibrate.
    ZRCSDKERR_CAMERA_CAN_NOT_DIRECTOR_CALIBRATE = 133,               ///<Camera can not director calibrate.
    ZRCSDKERR_CAMERA_THIS_CAMERA_NOT_SUPPORT_CALIBRATION = 134,      ///<The camera not support calibration.
    ZRCSDKERR_CAMERA_CALIBRATION_IN_WRONG_ORDER = 135,               ///<Calibration in wrong order.
    ZRCSDKERR_CAMERA_MULTI_CAMERA_NOT_ENABLE = 136,                  ///<Multi-camera function not enable.
    ZRCSDKERR_CAMERA_COUNT_OF_MULTI_CAMERAS_REACH_MAX = 137,         ///<The count of multi cameras reaches max.
    ZRCSDKERR_CAMERA_CAN_NOT_BE_SELECTED = 138,                      ///<The camera can't be selected for multi-camera.
    ZRCSDKERR_CAMERA_NOT_EXIST = 139,                                ///<The camera can't be found.
    ZRCSDKERR_CAMERA_CAN_NOT_SET_COM_ID = 140,                       ///<Camera can't set com ID.
    ZRCSDKERR_CAMERA_CAN_NOT_SET_DISPLAY_NAME = 141,                 ///<Camera can't set display name.
    ZRCSDKERR_CAMERA_NOT_SUPPORT_BOUNDARY = 142,                     ///<Not support camera boundary.
    ZRCSDKERR_CAMERA_BOUNDARY_CONFIGURATION_IN_WRONG_ORDER = 143,    ///<Camera boundary configuration is in wrong order.
    ZRCSDKERR_NDI_CAMERA_NOT_SELECTABLE = 144,                       ///<This NDI camera is currently being shared and cannot be selected.

    // Virtual Background Code
    ZRCSDKERR_VB_CAN_NOT_CHANGE_VB_WHEN_SETTING_LOCKED_ON_WEB = 180,        ///<Can not change virtual background when setting is locked on web.
    ZRCSDKERR_VB_CAN_NOT_CHANGE_VB_WHEN_IN_IMMERSIVE_CUTOUT = 181,          ///<Can not change virtual background when ZR is in immersive cutout.
    ZRCSDKERR_VB_CAN_NOT_CHANGE_TO_NONE_WHEN_VB_FORCE_ENABLED = 182,        ///<Can not change virtual background when virtual background is force enabled on web.
    ZRCSDKERR_VB_NOT_SUPPORT_BLUR_ALGORITHM = 183,                          ///<Try to change virtual background to blur but blur algorithm is not supported.
    // Meeting Error Code
    ZRCSDKERR_INVALID_MEETING = 200,                                 ///<Meeting is not valid.
    ZRCSDKERR_INVALID_MEETING_NUMBER = 201,                          ///<Meeting number is not valid.
    ZRCSDKERR_INVALID_THIRD_PARTY_MEETING = 202,                     ///<The meeting is not a third party meeting.

    ZRCSDKERR_NONE_UNENCRYPTED_ATTENDEE = 210,                       ///<None unencrypted attendee in webinar.
    ZRCSDKERR_NOT_AES256_GCM_ENCRYPTED_MEETING = 211,                ///<Not a AES256 GCM encrypted meeting.
    ZRCSDKERR_MEETING_EXIST_THIRD_PARTY_AUDIO = 212,                 ///<Meeting exist third party audio,

    ZRCSDKERR_ALREADY_IN_MEETING = 220,                              ///<Already in meeting.
    ZRCSDKERR_NOT_IN_MEETING = 221,                                  ///<Not in meeting
    ZRCSDKERR_NOT_IN_WEBINAR = 222,                                  ///<Not in webinar
    ZRCSDKERR_NOT_IN_PSTN_CALLOUT_MEETING = 223,                     ///<Not in a pstn call out meeting
    ZRCSDKERR_NOT_IN_E2EE_Meeting = 224,                             ///<Not in end-to-end encrypted meeting.
    ZRCSDKERR_NOT_IN_INTEGRATION_MEETING = 225,                      ///<Not in integration meeting.
    ZRCSDKERR_NOT_IN_WEBINAR_PRACTICE_SESSION = 226,                 ///<Not in webinar practice session.
    ZRCSDKERR_MEETING_NOT_SUPPORT_WAITING_ROOM = 227,                ///<Meeting not support waiting room.
    ZRCSDKERR_MEETING_WAITING_ROOM_LOCKED = 228,                     ///<Meeting waiting room is locked.
    ZRCSDKERR_CAN_NOT_JOIN_VIA_WEB_CLIENT = 229,                     ///<You can't join the meeting via web client.
    ZRCSDKERR_IN_E2EE_MEETING = 230,                                 ///<In an end-to-end encrypted meeting.
    ZRCSDKERR_IN_WAITING_ROOM = 231,                                 ///<In waiting room.
    ZRCSDKERR_IN_BACKSTAGE_DEBRIEF_SESSION = 232,                    ///<In webinar backstage debrief session.
    ZRCSDKERR_SELF_OR_USER_IN_IMPROPER_POSITION_IN_WEBINAR = 233,    ///<Self or the user is not in proper position(main meeting or backstage) in webinar.
    ZRCSDKERR_MEETING_CHAT_NOT_SUPPORT_OR_ENABLE = 234,              ///<Not support or enable meeting chat.
    ZRCSDKERR_NOT_SUPPORT_CHAT_PRIVILEGE_CONTROL = 235,              ///<Not support chat privilege control.
    ZRCSDKERR_THIS_INCOMING_MEETING_CALL_NOT_EXIST = 236,            ///<This incoming meeting call not exist.
    ZRCSDKERR_CAN_NOT_JOIN_VIA_SIP = 237,                            ///<You can't join the meeting via SIP.
    ZRCSDKERR_CAN_NOT_JOIN_VIA_PHONE = 238,                          ///<You can't join the meeting via phone.
    ZRCSDKERR_NOT_SUPPORT_CURRENT_INTEGRATION_MEETING = 239,         ///<Not support current integration meeting.

    // View Layout Error Code
    ZRCSDKERR_VIEW_LAYOUT_FOLLOWING_HOST_ORDER = 250,                ///<Current view layout is following host order.
    ZRCSDKERR_SCREEN_CAN_NOT_BE_PINNED = 251,                        ///<Screen cannot be pinned
    ZRCSDKERR_USER_ALREADY_PINNED_ON_SCREEN = 252,                   ///<User video is already been pinned on screen,
    ZRCSDKERR_USER_ALREADY_BE_SPOTLIGHTED = 253,                     ///<User is already been spotlighted
    ZRCSDKERR_MEETING_NOT_SUPPORT_MULTI_PIN = 254,                   ///<The meeting doesn't support multi-pin.
    ZRCSDKERR_NO_USER_PINNED = 255,                                  ///<No user pinned on screen, use PinUserOnScreen instead.
    ZRCSDKERR_REACH_MAX_PINNED_USERS_COUNT = 256,                    ///<Screen's pinned users count reach max limit.
    ZRCSDKERR_NO_USER_SPOTLIGHTED = 257,                             ///<No user pinned on screen, use PinUserOnScreen instead.
    ZRCSDKERR_REACH_MAX_SPOTLIGHTED_USERS_COUNT = 258,               ///<Screen's spotlighted users count reach max limit.
    ZRCSDKERR_WRONG_NUMBER_OF_SPOTLIGHTED_USERS = 279,               ///<The count of spotlighted users does not meet the conditions.
    ZRCSDKERR_NOT_REACH_MIN_USERS_FOR_SPOTLIGHT = 260,               ///<The count of users in meeting does not meet the conditions.
    ZRCSDKERR_NDI_NOT_ENABLE = 261,                                  ///<NDI not enable.
    ZRCSDKERR_NDI_SOURCE_HAS_BEEN_PINNED = 262,                      ///<The NDI source has been pinned.
    ZRCSDKERR_NOT_IN_DYNAMIC_LAYOUT = 263,                           ///<Not in dynamic layout.
    ZRCSDKERR_NO_SHARE_WHILE_IN_LIVE_FEED = 264,                     ///<No share source while in live feed.
    ZRCSDKERR_CANNOT_ADJUST_ONLY_ONE_SCREEN = 265,                   ///<Cannot adjust screen while ZR only has one screen.
    ZRCSDKERR_CANNOT_SET_MAIN_SCREEN_AS_CONFIDENCE_MONITOR = 266,    ///<Cannot use main screen as confidence monitor.
    ZRCSDKERR_ADJUST_SCREEN_POSITION_OUT_OF_RANGE = 267,             ///<Adjust screen position out of range.
    ZRCSDKERR_INDEX_NOT_PERSIST_NDI = 268,                           ///<Index not persist NDI.

    // Share Error Code
    ZRCSDKERR_MEETING_SHARE_IS_DISABLED = 300,                       ///<Meeting Share is disabled.
    ZRCSDKERR_NOT_IN_SHARING = 301,                                  ///<Not in sharing.
    ZRCSDKERR_NOT_SHARING_BLACKMAGIC = 302,                          ///<Blackmagic is not in sharing
    ZRCSDKERR_NOT_VIEWING_BLACKMAGIC_LOCALLY = 303,                  ///<Blackmagic is not viewing locally
    ZRCSDKERR_SHARING_BLACKMAGIC_DATA_NOT_READY = 304,               ///<Blackmagic data is not ready to share
    ZRCSDKERR_VIEWING_BLACKMAGIC_LOCALLY_DATA_NOT_READY = 305,       ///<Blackmagic data is not ready to view locally
    ZRCSDKERR_NO_CAMERA_IS_IN_SHARING = 306,                         ///<No camera is in sharing
    ZRCSDKERR_PIN_SHARE_NOT_DOUBLE_CONFIRMED = 307,                  ///<Current pin share action needs to be double-confirmed.
    ZRCSDKERR_SHARE_AUDIO_CONTROL_NOT_SUPPORT_ON_SINGLE_SCREEN = 308,///<Share audio control is not supported on single screen.
    ZRCSDKERR_SHARE_NOT_WITH_AUDIO = 309,                            ///<Share not with audio.
    ZRCSDKERR_NOT_AVAILABLE_WHILE_IN_MULTI_SHARE = 310,              ///<Feature is disabled while in multi share mode.
    ZRCSDKERR_SCREEN_CAN_NOT_PIN_SHARE = 311,                        ///<Screen cannot be pinned share.
    ZRCSDKERR_CAN_NOT_PIN_BACKSTAGE_SHARE_IN_WEBINAR = 312,          ///<Cannot pin backstage share in webinar.
    ZRCSDKERR_THIS_CAMERA_NOT_SHAREABLE = 313,                       ///<This NDI camera is currently being selected and cannot be shared.
    ZRCSDKERR_CANNOT_PIN_THE_SHARE_ON_CONFIDENCE_MONITOR_SCREEN = 314, ///<The share can't be pinned on confidence monitor screen.

    // Recording Error Code
    ZRCSDKERR_RECORDING_NOT_AVAILABLE_IN_MEETING = 350,              ///<Recording is not available in current meeting.
    ZRCSDKERR_NOT_CONFIRM_START_RECORDING_DISCLAIMER = 351,          ///<You can't start recording before confirming start recording disclaimer.
    ZRCSDKERR_NOT_SET_RECORDING_NOTIFICATION_EMAIL = 352,            ///<You can't start recording before setting recording notification email.
    ZRCSDKERR_NO_NEED_TO_PROMPT_START_RECORDING_DISCLAIMER = 353,    ///<No need to prompt start recording disclaimer

    // User Error Code
    ZRCSDKERR_USER_NOT_FOUND_BY_ID = 400,                            ///<Can not find the participant by provided userID.
    ZRCSDKERR_USER_NOT_FOUND_BY_GUID = 401,                          ///<User info not found by guid.
    ZRCSDKERR_USER_IS_MULTI_STREAM_VIDEO_USER = 402,                 ///<Multi stream video user not supports some actions.
    ZRCSDKERR_USER_IS_PHONE_CALL_IN_USER = 403,                      ///<Pure call in user not supports some actions.
    ZRCSDKERR_USER_IS_PANELIST = 404,                                ///<User is panelist.
    ZRCSDKERR_USER_IS_ATTENDEE = 405,                                ///<User is attendee.
    ZRCSDKERR_USER_IN_BACKSTAGE = 406,                               ///<The user is in backstage.
    ZRCSDKERR_USER_CAN_NOT_BE_ALLOWED_RECORDING = 407,               ///<The user can't be allowed to record, because this user has a special identity, such as host, cohost.
    ZRCSDKERR_USER_CAN_NOT_BE_ALLOWED_MULTI_PIN = 408,               ///<The user can't be allowed to multi-pin, because this user has a special identity, such as host, cohost.
    ZRCSDKERR_USER_CAN_NOT_BE_SPOTLIGHTED = 409,                     ///<User can not be spotlighted
    ZRCSDKERR_USER_CAN_NOT_BE_ADD_PINNED = 410,                      ///<The user can't be add-pinned.
    ZRCSDKERR_USER_CAN_NOT_BE_ADD_SPOTLIGHTED = 411,                 ///<The user can't be add-spotlighted.
    ZRCSDKERR_USER_IS_SIMULIVE_USER = 412,                           ///<The user is simulive user.
    ZRCSDKERR_USER_UNSUPPORTED_ACTION = 413,                        ///<The user unsupported action.

    // Caption/Transcription Error Code
    ZRCSDKERR_CLOSED_CAPTION_NOT_AVAILABLE = 450,                              ///<Closed caption is not available.
    ZRCSDKERR_CLOSED_CAPTION_DISABLED_BECAUSE_NEW_LTT_FEATURE_ON = 451,        ///<Closed caption is disabled, use New LTT caption instead.
    ZRCSDKERR_LIVE_TRANSCRIPTION_NOT_ENABLED = 452,                            ///<Live transcription is not enabled.
    ZRCSDKERR_LIVE_TRANSCRIPTION_DISABLED_BECAUSE_NEW_LTT_FEATURE_ON = 453,    ///<Live transcription is disabled, use New LTT caption instead.

    ZRCSDKERR_NEW_LTT_CAPTION_NOT_ENABLED = 460,                               ///<New LTT Caption feature is not enabled.
    ZRCSDKERR_NEW_LTT_CAPTION_TRANSLATED_CAPTION_NOT_ENABLED = 461,            ///<New LTT Caption translated caption feature is not enabled.
    ZRCSDKERR_NEW_LTT_CAPTION_SPEAKING_LANGUAGE_NOT_AVAILABLE = 462,           ///<New LTT Caption speaking language is not available.
    ZRCSDKERR_NEW_LTT_CAPTION_TRANSLATION_LANGUAGE_NOT_AVAILABLE = 463,        ///<New LTT Caption translation language is not available.

    // Breakout Room Error Code
    ZRCSDKERR_BO_NO_PERMISSION_TO_MANAGE_BO = 501,            ///<No permission to manage breakout room.
    ZRCSDKERR_BO_CAN_NOT_CREATE_BO_AFTER_STARTED = 502,       ///<Can't create breakout room when bo is started or stopping.
    ZRCSDKERR_BO_CAN_NOT_EDIT_BO_AFTER_STARTED = 503,         ///<Can't edit breakout room when bo is started or stopping.
    ZRCSDKERR_BO_CREATE_ROOM_COUNT_EXCEED_MAX = 504,          ///<Create breakout room count exceeds max room count.
    ZRCSDKERR_BO_NOT_ENABLED_WEB_PRE_ASSIGN = 505,            ///<Web pre-assign bo not enabled.
    ZRCSDKERR_BO_BROADCAST_MESSAGE_IS_DISABLED = 506,         ///<Broadcast message to breakout room is disabled.
    ZRCSDKERR_BO_CAN_NOT_DELETE_MAIN_SESSION = 507,           ///<Can't delete breakout room main session.
    ZRCSDKERR_BO_NO_BREAKOUT_ROOM_ADDED = 508,                ///<No breakout rooms added.
    ZRCSDKERR_BO_ROOM_INFO_NOT_FOUND_BY_BID = 509,            ///<Can't found the correspond breakout room info with sessionBID.
    ZRCSDKERR_BO_NOT_IN_PROGRESS = 511,                       ///<Breakout Room is not in progress.
    ZRCSDKERR_BO_NO_PERMISSION_JOIN_UNASSIGNED_ROOM = 514,    ///<You have no permission to join unassigned breakout room.
    ZRCSDKERR_BO_NO_PERMISSION_LEAVE_BO = 515,                ///<No permission to leave breakout room.
    ZRCSDKERR_BO_NOT_IN_BREAKOUT_ROOM = 516,                  ///<Not in a breakout room.
    ZRCSDKERR_BO_NOT_IN_MAIN_SESSION = 517,                   ///<Not in main session.
    ZRCSDKERR_BO_MANAGER_CAN_NOT_ASK_FOR_HELP = 518,          ///<You are the breakout room manager.
    ZRCSDKERR_BO_USER_UNASSIGNED_TO_BREAKOUT_ROOM = 519,      ///<User is not assigned to any breakout rooms.
    ZRCSDKERR_BO_IN_STOPPING = 520,                           ///<BO is in stopping.
    ZRCSDKERR_BO_REACH_CAPACITY = 521,                        ///<Reaching capacity.

    // BYOD Error Code
    ZRCSDKERR_NOT_SUPPORT_BYOD_MODE = 550,                       ///<Not support BYOD mode.
    ZRCSDKERR_NOT_SUPPORT_BYOD_CAMERA_CONTROL = 551,             ///<Not support BYOD camera control.
    ZRCSDKERR_NOT_SUPPORT_BYOD_MICROPHONE_CONTROL = 552,         ///<Not support BYOD microphone control.
    ZRCSDKERR_NOT_SUPPORT_BYOD_SPEAKER_CONTROL = 553,            ///<Not support BYOD speaker control.
    ZRCSDKERR_NOT_SUPPORT_BYOD_AUTO_FRAMING = 554,               ///<Not support BYOD auto framing.
    ZRCSDKERR_BYOD_NOT_SUPPORT_MANUAL_OPERATE = 555,             ///<BYOD mode not support manual operate.
    ZRCSDKERR_NOT_IN_RIGHT_BYOD_STATUS = 556,                    ///<Not in right BYOD status.
    ZRCSDKERR_BYOD_HDMI_SHARE_NOT_SUPPORT_OR_NOT_READY = 557,    ///<BYOD HDMI share not support or not ready.

    // Polling Error
    ZRCSDKERR_POLLING_INACTIVE = 600,                                     ///<Polling session is inactive.
    ZRCSDKERR_POLLING_ALREADY_EXIST_INCHARGE_DEVICE = 601,                ///<Already exist incharge device, you cannot grab control.
    ZRCSDKERR_POLLING_NOT_INCHARGE_DEVICE = 602,                          ///<Not the incharge device of polling, take in charge before action.
    ZRCSDKERR_POLLING_INVALID_QUESTION_TYPE = 603,                        ///<Question type is invalid.
    ZRCSDKERR_POLLING_MISSING_QUESTION_ID = 604,                          ///<Question ID is absent.
    ZRCSDKERR_POLLING_MISSING_QUESTION_INDEX = 605,                       ///<Question index is absent.
    ZRCSDKERR_POLLING_MISSING_QUESTION_ANSWERS = 606,                     ///<Question's answers are absent.
    ZRCSDKERR_POLLING_MISSING_ANSWER_ID = 607,                            ///<Question's answer's ID is absent.
    ZRCSDKERR_POLLING_ANSWERS_COUNT_ERROR = 608,                          ///<Question's answers count error.
    ZRCSDKERR_POLLING_SINGLE_CHOICE_CAN_ONLY_CHOOSE_ONE_ANSWER = 609,     ///<Single Choice question can only choose one answer.
    ZRCSDKERR_POLLING_MULTI_CHOICE_SHOULD_CHOOSE_AT_LEAST_ONE = 610,      ///<Multi Choice's question should choose at least one answer.
    ZRCSDKERR_POLLING_MISSING_SUB_QUESTIONS = 611,                        ///<Sub-question missing.
    ZRCSDKERR_POLLING_MISSING_SUB_QUESTIONS_ID = 612,                     ///<Sub-question ID is absent.
    ZRCSDKERR_POLLING_MISSING_SUB_QUESTIONS_INDEX = 613,                  ///<Sub-question index is absent.
    ZRCSDKERR_POLLING_MISSING_SUB_QUESTIONS_ANSWERS = 614,                ///<Sub-question answers are absent.
    ZRCSDKERR_POLLING_MISSING_SUB_QUESTIONS_ANSWER_ID = 615,              ///<Sub-question answers are absent.
    ZRCSDKERR_POLLING_SUB_QUESTIONS_ANSWERS_COUNT_ERROR = 616,            ///<Sub-question answers are absent.
    ZRCSDKERR_POLLING_SUB_QUESTIONS_ANSWERS_CAN_ONLY_CHOOSE_ONE = 617,    ///<Sub-question answers can only choose one.
    ZRCSDKERR_POLLING_TEXT_ANSWER_EMPTY = 618,                            ///<Text answer is absent.
    ZRCSDKERR_POLLING_TEXT_ANSWER_UNDER_SIZE = 619,                       ///<Text answer under min character size limit.
    ZRCSDKERR_POLLING_TEXT_ANSWER_OVER_SIZE = 620,                        ///<Text answer over max character size limit.
    ZRCSDKERR_POLLING_NOT_FILL_IN_ALL_BLANK = 621,                        ///<Not fill in all blank.
    ZRCSDKERR_POLLING_NOT_SUPPORT_SHOW_POLL_OVERLAY = 622,                ///<ZR Not support show poll overlay.
    ZRCSDKERR_POLLING_SHOW_POLL_OVERLAY_NOT_ENABLED = 623,                ///<Show poll overlay not enabled.

    ZRCSDKERR_SIP_THIS_INCOMING_CALL_NOT_EXIST = 650,                       ///<The incoming call is not in incoming calls.
    ZRCSDKERR_SIP_ALREADY_HAVE_CALL = 651,                                  ///<Already have a SIP call.
    ZRCSDKERR_SIP_NO_CALL = 652,                                            ///<No SIP call in current time.
    ZRCSDKERR_SIP_THIS_CALL_NOT_IN_SIP_CALL_LIST = 653,                     ///<The SIP call is not in SIP calls.
    ZRCSDKERR_SIP_THIS_CALL_NOT_IN_AN_CORRECT_STATE = 654,                  ///<The SIP call is not in correct state.
    ZRCSDKERR_SIP_FOREGROUND_NOT_IN_CALL = 655,                             ///<Foreground call is not in call.
    ZRCSDKERR_SIP_NOT_IN_SIP_CALL_ACTIVITY = 656,                           ///<Not in SIP call activity.
    ZRCSDKERR_SIP_SERVICE_NOT_AVAILABLE = 657,                              ///<SIP service is not available.
    ZRCSDKERR_SIP_ZOOM_PHONE_NOT_AVAILABLE = 658,                           ///<Zoom Phone is not available.
    ZRCSDKERR_SIP_NUMBER_OF_CALLS_EXCEEDED_LIMIT = 659,                     ///<Number of calls exceeds limit.
    ZRCSDKERR_SIP_ALREADY_HAVE_MERGED_CALL = 660,                           ///<Already have merged call.
    ZRCSDKERR_SIP_THIS_MERGED_CALL_CAN_NOT_BE_MERGED = 661,                 ///<This call can not be merged.
    ZRCSDKERR_SIP_THIS_CALL_IS_TRANSFERRING = 662,                          ///<The call is transferring.
    ZRCSDKERR_SIP_ALREADY_HAVE_WARM_TRANSFERRING_CALL = 663,                ///<Already have warm transferring call.
    ZRCSDKERR_SIP_CAN_NOT_TRANSFER_MERGED_CALL = 664,                       ///<Can not transfer merged call.
    ZRCSDKERR_SIP_THIS_CALL_HAS_NO_RELATED_CALL_ID = 665,                   ///<The call has no related call ID.
    ZRCSDKERR_SIP_RELATED_CALL_NOT_EXIST = 666,                             ///<Related call does not exist.
    ZRCSDKERR_SIP_HAVE_NO_FOREGROUND_CALL = 667,                            ///<Have no foreground call.
    ZRCSDKERR_SIP_THIS_CALL_IS_EMERGENCY_CALL = 668,                        ///<The call is emergency call.
    ZRCSDKERR_SIP_CAN_NOT_CHANGE_LOCATION_PERMISSION_IN_MEETING = 669,      ///<Can not change location permission in meeting.
    ZRCSDKERR_SIP_NO_EMERGENCY_INFO = 670,                                  ///<No emergency info.
    ZRCSDKERR_SIP_NO_UNHOLD_CALL = 671,                                     ///<No unhold call.
    ZRCSDKERR_SIP_NOT_ALL_SIP_CALLS_IN_CALL = 672,                          ///<Not all SIP calls in call.

    // AI Companion
    ZRCSDKERR_AIC_MEETING_SUMMARY_DISABLED = 700,                       ///<AI Companion meeting summary disabled.
    ZRCSDKERR_AIC_MEETING_QUERY_DISABLED = 701,                         ///<AI Companion meeting query disabled.
    ZRCSDKERR_AIC_NO_PRIVILEGE_SETTING_ITEM = 702,                      ///<AI Companion no privilege setting item.
    ZRCSDKERR_AIC_INVALID_PRIVILEGE_SETTING_ITEM = 703,                 ///<AI Companion invalid privilege setting item.
    ZRCSDKERR_AIC_ALREADY_SET_MEETING_SUMMARY_NOTIFY_EMAIL = 704,       ///<AI Companion already set meeting summary notify email.
    ZRCSDKERR_AIC_NOT_SET_MEETING_SUMMARY_NOTIFY_EMAIL = 705,           ///<AI Companion not set meeting summary notify email.

    // Meeting Chat
    ZRCSDKERR_MEETING_CHAT_DISABLED = 750,                              ///<Meeting chat disabled
    ZRCSDKERR_MEETING_CHAT_NOT_NEW_CHAT_EXPERIENCE = 751,               ///<Not new meeting chat
    ZRCSDKERR_MEETING_CHAT_REQUIRE_TO_CONFIRM_CMC_TIP = 752,            ///<Require to confirm CMC tip
    ZRCSDKERR_MEETING_CHAT_NO_NEED_TO_CONFIRM_CMC_TIP = 753,            ///<No need to confirm CMC tip
    ZRCSDKERR_MEETING_CHAT_EMPTY_MESSAGE_CONTENT = 754,                 ///<Sending message content empty
    ZRCSDKERR_MEETING_CHAT_MESSAGE_CONTENT_REACH_MAX_SIZE = 755,        ///<Sending message content reach max size
    ZRCSDKERR_MEETING_CHAT_THREAD_NOT_FOUND = 756,                      ///<Thread not found
    ZRCSDKERR_MEETING_CHAT_MESSAGE_NOT_FOUND = 757,                     ///<Message not found
    ZRCSDKERR_MEETING_CHAT_EMOJI_NOT_EXIST = 758,                       ///<Emoji not found
    ZRCSDKERR_MEETING_CHAT_CAN_NOT_SEND_MESSAGE = 759,                  ///<Can not send message
    ZRCSDKERR_MEETING_CHAT_THREAD_CAN_NOT_REPLY = 760,                  ///<Can not reply message
    ZRCSDKERR_MEETING_CHAT_MESSAGE_CAN_NOT_DELETE = 761,                ///<Can not delete message
    ZRCSDKERR_MEETING_CHAT_MESSAGE_CAN_NOT_EDIT = 762,                  ///<Can not edit message
    ZRCSDKERR_MEETING_CHAT_MESSAGE_CAN_NOT_REACT_WITH_EMOJI = 763,      ///<Can not react message with emoji
    ZRCSDKERR_MEETING_CHAT_PLEASE_USE_NEW_CHAT_API = 764,               ///<Please use new chat APIs
    ZRCSDKERR_MEETING_CHAT_COMMENT_TYPE_NOT_SAME_WITH_THREAD_TYPE = 765,///<Reply in thread must same message type
    ZRCSDKERR_MEETING_CHAT_PRIVATE_THREAD_USER_ID_ERROR = 766,          ///<Private chat user id error.
    ZRCSDKERR_MEETING_CHAT_ONLY_ALLOW_REACT_WITH_SELECTED_EMOJIS = 767, ///<Can not react message with non-selected emojis.

    // HWIO Error Code
    ZRCSDKERR_HWIO_SERVICE_UNAVAILABLE = 780,                           ///<HWIO service is unavailable.

    // Q&A Error Code
    ZRCSDKERR_QA_NOT_MAIN_ZRC = 800,                                    ///<Not main controller.
    ZRCSDKERR_QA_NOT_ALLOW_SEND_QUESTION = 801,                         ///<Not allow to send question.
    ZRCSDKERR_QA_NOT_ALLOW_SEND_QUESTION_ANONYMOUS = 802,               ///<Not allow to send question anonymous.
    ZRCSDKERR_QA_NOT_ALLOW_ANSWER_QUESTION = 803,                       ///<Not allow to answer question.
    ZRCSDKERR_QA_NOT_ALLOW_COMMENT_QUESTION = 804,                      ///<Not allow to comment question.
    ZRCSDKERR_QA_NOT_ALLOW_UPVOTE_QUESTION = 805,                       ///<Not allow to upvote question.
    
    // Pro AV Error Code
    ZRCSDKERR_NDI_HWIO_BOTH_DISABLED = 830,                          ///<NDI & HWIO service both disabled.
    
    ///Reaction Emoji Error Code
    ZRCSDKERR_REACTION_DISABLED = 850,                                  ///<Reaction disabled.
    
    ///Smart Tag Error Code
    ZRCSDKERR_SMART_TAG_DISABLED = 870,
    ZRCSDKERR_SMART_TAG_UNSUPPORTED = 871,
};

/*! \enum RoomUnpairedReason
    \brief Reason of why being unpaired from ZR
    Here are more detailed structural descriptions.
 */
enum RoomUnpairedReason
{
    RoomUnpairedReason_TokenInvalid = 1,      ///<Token is invalid.
    RoomUnpairedReason_RefreshTokenFail = 2,  ///<Internal auto refresh token failed.
    RoomUnpairedReason_RemovedByAdmin = 3,    ///<Current device is being removed from admin on web portal.
    RoomUnpairedReason_BetaBuildExpired = 4,  ///<Running an evaluation build which has expired.
    RoomUnpairedReason_ZRInE2EMeeting = 5,    ///<ZR in end-to-end encrytion meeting
};

////////////////////////////////////////// IPreMeetingService ///////////////////////////////////////

/*! \enum ConnectionState
    \brief SDK connect with Zoom Room state.
    Here are more detailed structural descriptions.
 */
enum ConnectionState
{
    ConnectionStateNone = -1,          ///<Invalid.
    ConnectionStateEstablished = 0,    ///<Connection established with Zoom Room.
    ConnectionStateConnected = 1,      ///<Connection connected and verified with Zoom Room.
    ConnectionStateDisconnected = 2    ///<Disconnected.
};

/*! \enum ScheduleCalendarEventResult
    \brief Schedule calendar event result.
    Here are more detailed structural descriptions.
 */
enum ScheduleCalendarEventResult
{
    ScheduleCalendarEventResultSuccess = 0,    ///<Success
    ScheduleCalendarEventResultFailUnknown,    ///<Fail, reason classified.
    ScheduleCalendarEventResultFailWeakPWD     ///<Fail, password too weak.
};

/*! \enum DeleteCalendarEventResult
    \brief Delete calendar event result.
    Here are more detailed structural descriptions.
 */
enum DeleteCalendarEventResult
{
    DeleteCalendarEventResultSuccess = 0,             ///<Success
    DeleteCalendarEventResultFailByDeleteCalendar,    ///<Fail, delete meeting failed
    DeleteCalendarEventResultFailByZRInMeeting,       ///<Fail, ZR is in meeting.
    DeleteCalendarEventResultFailUnknown,             ///<Fail, reason classified.
};

/*! \enum ListMeetingResult
    \brief List meeting result
    Here are more detailed structural descriptions.
 */
enum ListMeetingResult
{
    LIST_MEETING_SUCCESS = 0,                                   ///<Success.
    LIST_MEETING_ERROR_UNKNOWN,                                 ///<Error unknown.
    LIST_MEETING_ERROR_GOOGLE_CALENDAR_INVALID_CREDENTIAL,      ///<Google calendar invalid credential.
    LIST_MEETING_ERROR_GOOGLE_CALENDAR_DAILY_LIMIT_EXCEEDED,    ///<Google calendar daily limit exceeded.
    LIST_MEETING_ERROR_EWS_INVALID_CREDENTIAL,                  ///<EWS invalid credential.
    LIST_MEETING_ERROR_EWS_AUTH_METHOD_UNSUPPORTED,             ///<EWS auth method unsupported.
    LIST_MEETING_ERROR_EWS_FOLDER_NOT_FOUND,                    ///<EWS folder not found.
    LIST_MEETING_ERROR_EWS_IMPERSONATE_USER_DENIED,             ///<EWS impersonate user denied.
    LIST_MEETING_ERROR_EWS_NON_EXISTENT_MAILBOX,                ///<EWS non existent mailbox.
    LIST_MEETING_ERROR_CALENDAR_SERVICE_DISCONNECTED,           ///<Calendar service disconnected.
};

/*! \enum MeetingScheduleFrom
    \brief Meeting schedule from
    Here are more detailed structural descriptions.
 */
enum MeetingScheduleFrom
{
    MeetingScheduleFromUnknown = -1,           ///<Init
    MeetingScheduleFromCalendar = 0,           ///<Meeting schedule from calendar.
    MeetingScheduleFromScheduleDisplay = 1,    ///<Meeting schedule from Schedule Display
    MeetingScheduleFromInstantMeeting = 2      ///<Meeting schedule from Instant Meeting, Instant Meeting will auto create a calendar event.
};

/*! \enum ZoomMeetingItemType
    \brief Zoom meeting item type.
    Here are more detailed structural descriptions.
 */
enum ZoomMeetingItemType
{
    ZoomMeetingItemTypeDefault = 0,        ///<Calendar event, Zoom meeting, Webinar, Onzoom.
    ZoomMeetingItemTypeZESingleSession,    ///<Zoom event single session.
    ZoomMeetingItemTypeZEMultiSession,     ///<Zoom event multi session.
    ZoomMeetingItemTypeZESubSession        ///<Zoom event sub session.
};

/*! \enum MeetingEncryptionAlgorithm
    \brief Meeting encryption algorithm
    Here are more detailed structural descriptions.
 */
enum MeetingEncryptionAlgorithm
{
    MeetingEncryptionNone = 0,      ///<None
    MeetingEncryptionAES256,        ///<Encryption algorithm AES-256
    MeetingEncryptionAES256GCM      ///<Encryption algorithm AES-256-GCM
};

/*! \enum EncryptionAlgorithm
    \brief Encryption algorithm
    Here are more detailed structural descriptions.
 */
enum EncryptionAlgorithm
{
    EncryptionAlgorithm_Unknown = -1,           ///<Unknown
    EncryptionAlgorithm_None = 0,               ///<None
    EncryptionAlgorithm_AES256 = 1,             ///<AES 256
    EncryptionAlgorithm_AES256_GCM = 2,         ///<AES 256 GCM
    EncryptionAlgorithm_AES256_GCM_3rd = 3,     ///<AES 256 GCM 3rd
};

/*! \enum ThirdPartyMeetingServiceProvider
    \brief Third party meeting service provider
    Here are more detailed structural descriptions.
 */
enum ThirdPartyMeetingServiceProvider
{
    ThirdPartyMeetingServiceProviderInvalid = 0,           ///<Invalid
    ThirdPartyMeetingServiceProviderWebex = 1,             ///<Webex
    ThirdPartyMeetingServiceProviderSkype = 2,             ///<Skype
    ThirdPartyMeetingServiceProviderGoToMeeting = 3,       ///<Goto Meeting
    ThirdPartyMeetingServiceProviderTeams = 4,             ///<Teams
    ThirdPartyMeetingServiceProviderBluejeans = 5,         ///<Bluejeans
    ThirdPartyMeetingServiceProviderGoogleHangouts = 6,    ///<Google Hangouts
    ThirdPartyMeetingServiceProviderFuze = 7,              ///<Fuze
    ThirdPartyMeetingServiceProviderMetaWorkrooms = 8      ///<Meta workrooms
};

/*! \enum LegacyRoomSystemType
    \brief Legacy room system type.
    Here are more detailed structural descriptions.
 */
enum LegacyRoomSystemType
{
    LegacyRoomSystemTypeNone = 0,   ///<None
    LegacyRoomSystemTypeH323 = 1,   ///<H323
    LegacyRoomSystemTypeSIP = 2,    ///<SIP
    LegacyRoomSystemTypeBoth = 3    ///<Both SIP and H323
};

/*! \enum LegacyRoomSystemEncryptType
    \brief Leagacy room system encrypt type.
    Here are more detailed structural descriptions.
 */
enum LegacyRoomSystemEncryptType
{
    LegacyRoomSystemEncryptTypeNo = 0,      ///<Not encrypt
    LegacyRoomSystemEncryptTypeYes = 1,     ///<Yes
    LegacyRoomSystemEncryptTypeAuto = 2     ///<Auto
};

////////////////////////////////////////// ISettingService //////////////////////////////////////////

/*! \enum DeviceType
    \brief Device type.
    Here are more detailed structural descriptions.
 */
enum DeviceType
{
    DeviceTypeMicrophone = 0,    ///<Device type microphone.
    DeviceTypeSpeaker = 1,       ///<Device type speaker.
    DeviceTypeCamera = 2         ///<Device type camera.
};

/*! \enum CameraBoundaryAdjustField
    \brief Camera boundary adjust field
    Here are more detailed structural descriptions.
 */
enum CameraBoundaryAdjustField
{
    CameraBoundaryAdjustFieldUnknown = -1,  ///<Unknown.
    CameraBoundaryAdjustFieldLeft = 0,      ///<Camera boundary adjust field left.
    CameraBoundaryAdjustFieldRight,         ///<Camera boundary adjust field right.
    CameraBoundaryAdjustFieldDepth          ///<Camera boundary adjust field depth.
};
////////////////////////////////////////// IMeetingService //////////////////////////////////////////


/*! \enum MeetingType
    \brief Meeting type.
    Here are more detailed structural descriptions.
 */
enum MeetingType
{
    MeetingTypeNone,           ///<For initialization.
    MeetingTypeMeeting,        ///<Meeting, Webinar, BreakoutRoom.
    MeetingTypeSharing,        ///<Local Presentation Meeting.
    MeetingTypePSTNCallout,    ///<PSTN Callout Meeting.
    MeetingTypeIntegration,    ///<Web Client Integration Meeting.
};

/*! \enum MeetingStatus
    \brief Current ZR's meeting status.
    Here are more detailed structural descriptions.
 */
enum MeetingStatus
{
    MeetingStatusNotInMeeting = 0,       ///<ZR is not in meeting.
    MeetingStatusConnectingToMeeting,    ///<ZR is connecting to meeting.
    MeetingStatusInMeeting,              ///<ZR is in meeting.
    MeetingStatusLoggedOut               ///<ZR is logged out.
};

/*! \enum ConfInstType
    \brief Conf inst type where source data is from.
    Here are more detailed structural descriptions.
*/
enum ConfInstType
{
    ConfInstTypeUnknown = -1,       ///<Unknown
    ConfInstTypeCurrentConf = 0,    ///<Current conference.
    ConfInstTypeMasterConf,         ///<Breakout Room's master conference.
    ConfInstTypeBackstage,           ///<Webinar's backstage.
    ConfInstTypeNewBO
};

/*! \enum PSTNCallOutStatus
    \brief PSTN callout status
    Here are more detailed structural descriptions.
 */
enum PSTNCallOutStatus
{
    PSTNCallOutStatusUnknown = 0,                 ///<Init
    PSTNCallOutStatusCalling = 1,                 ///<Calling
    PSTNCallOutStatusRinging = 2,                 ///<Ringing
    PSTNCallOutStatusAccepted = 3,                ///<Accepted
    PSTNCallOutStatusBusy = 4,                    ///<Busy
    PSTNCallOutStatusNotAvailable = 5,            ///<Not available
    PSTNCallOutStatusUserHangUp = 6,              ///<User hang up
    PSTNCallOutStatusOtherFail = 7,               ///<Other fail
    PSTNCallOutStatusJoinSuc = 8,                 ///<Join successfully
    PSTNCallOutStatusTimeOut = 9,                 ///<Timeout
    PSTNCallOutStatusZoomStartCancelCall = 10,    ///<Zoom start cancel call
    PSTNCallOutStatusZoomCallCanceled = 11,       ///<Zoom call canceled
    PSTNCallOutStatusZoomCancelCallFail = 12,     ///<Zoom cancel call failed.
    PSTNCallOutStatusNoAnswer = 13,               ///<No answer, indicate the phone ring but no-one answer
    PSTNCallOutStatusBlockNoHost = 14,            ///<When join before host case, disable international callout before host join
    PSTNCallOutStatusBlockHighRate = 15,          ///<The price of callout phone number is too expensive which has been blocked by system
    PSTNCallOutStatusBlockTooFrequent = 16        ///<Invite by phone with pressONE required, but invitee frequently does NOT press one then timeout
};

/*! \enum AudioType
    \brief Audio type
    Here are more detailed structural descriptions.
 */
enum AudioType
{
    AudioTypeNone,    ///<Audio type none
    AudioTypeVoIP,    ///<Audio type VoIP
    AudioTypePhone    ///<Audio type Phone
};

/*! \enum VideoSendingSourceType
    \brief Video sending source type
    Here are more detailed structural descriptions.
 */
enum VideoSendingSourceType
{
    VideoSendingSourceTypeDevice = 0,    ///<Video sending source physical device.
    VideoSendingSourceTypeLipsync = 1    ///<Video sending source Lip-sync.
};

/*! \enum MeetingScreen
    \brief Meeting screen index
    Here are more detailed structural descriptions.
 */
enum MeetingScreen
{
    MeetingScreenUnknown = -1,      ///<Invalid
    MeetingScreenFirst = 0,         ///<The first screen.
    MeetingScreenSecond = 1,        ///<The second screen.
    MeetingScreenThird = 2,         ///<The third screen.
    MeetingScreenConfidence = 100   ///<The confidence monitor.
};

/*! \enum ScreenLayoutSourceType
    \brief Zoom Room Screen layout source type.
    Here are more detailed structural descriptions.
 */
enum ScreenLayoutSourceType
{
    ScreenLayoutSourceTypeNone = -1,
    ScreenLayoutSourceTypeActiveVideo = 0,      ///<Active video
    ScreenLayoutSourceTypeSelfVideo,            ///<Self video
    ScreenLayoutSourceTypePinnedVideo,          ///<Pinned video
    ScreenLayoutSourceTypeSpotlight,            ///<Spotlight user
    ScreenLayoutSourceTypeGallery,              ///<Gallery view
    ScreenLayoutSourceTypeSharedContent,        ///<Shared content
    ScreenLayoutSourceTypeBackground,           ///<Background image
    ScreenLayoutSourceTypeLocalView,            ///<Local view
    ScreenLayoutSourceTypeImmersiveView,        ///<Immersive view
    ScreenLayoutSourceTypeZoomAppsView,         ///<Zoom Apps view
    ScreenLayoutSourceTypeDynamicView,          ///<Dynamic view
    ScreenLayoutSourceTypeThumbnailView,        ///<Thumbnail view
    ScreenLayoutSourceTypeThumbnailShareView    ///<Thumbnail share view
};

/*! \enum ShareSourceType
    \brief Share source type.
    Here are more detailed structural descriptions.
 */
enum ShareSourceType
{
    ShareSourceTypeUnknown = -1,                ///<Unknown
    ShareSourceTypeNormal = 0,                  ///<HDMI / airplay share / classic whiteboard / camera / whiteboard camera / Zapp share / screen share from others
    ShareSourceTypeCloudWB = 1,                 ///<Cloud whiteboaed
    ShareSourceTypeCollaborationZapps = 2,      ///<Collaboration  Zapps
};

/*! \enum SharingInstructionDisplayState
    \brief Sharing instruction display state.
    Here are more detailed structural descriptions.
 */
enum SharingInstructionDisplayState
{
    SharingInstructionDisplayStateNone = 0,           ///<Sharing instruction display state none.
    SharingInstructionDisplayStateDesktop,            ///<Sharing instruction display state desktop.
    SharingInstructionDisplayStateIOS,                ///<Sharing instruction display state iPhone/iPad.
    SharingInstructionDisplayStateWhiteboardCamera    ///<Sharing instruction display state whiteboard camera.
};

/*! \enum RoomSystemProtocolType
    \brief Room system protocol type
    Here are more detailed structural descriptions.
 */
enum RoomSystemProtocolType
{
    RoomSystemProtocolTypeUnknown = 0,    ///<Unknown
    RoomSystemProtocolTypeH323,           ///<H323
    RoomSystemProtocolTypeSIP             ///<SIP
};

/*! \enum ReactionFeedback
    \brief Reaction feedback
    Here are more detailed structural descriptions.
*/
enum ReactionFeedback
{
    ReactionFeedbackNone,      ///<None
    ReactionFeedbackHand,      ///<Hand Raised
    ReactionFeedbackYes,       ///<Yes
    ReactionFeedbackNo,        ///<No
    ReactionFeedbackFast,      ///<Speed up
    ReactionFeedbackSlow,      ///<Slow down
    ReactionFeedbackCoffee,    ///<Coffee
};

/*! \enum HandSkinTone
    \brief Hand skin tone, for example "🙌":["🙌","🙌🏻","🙌🏼","🙌🏽","🙌🏾","🙌🏿"]
    Here are more detailed structural descriptions.
*/
enum HandSkinTone
{
    HandSkinToneUnknown = 0,    ///<Invalid
    HandSkinTone1 = 1,          ///<Default, :_T("") ,      🙌
    HandSkinTone2 = 2,          ///<Unicode:_T("1f3fb"),    🙌🏻
    HandSkinTone3 = 3,          ///<Unicode:_T("1f3fc"),    🙌🏼
    HandSkinTone4 = 4,          ///<Unicode:_T("1f3fd"),    🙌🏽
    HandSkinTone5 = 5,          ///<Unicode:_T("1f3fe"),    🙌🏾
    HandSkinTone6 = 6,          ///<Unicode:_T("1f3ff"),    🙌🏿
};

/*! \enum ReactionIntensity
    \brief Reaction intensity, how you see reactions on screen.
    Here are more detailed structural descriptions.
*/
enum ReactionIntensity
{
    ReactionIntensityHidden = 0,    ///<Reactions are hidden on screen.
    ReactionIntensityFull,          ///<Reactions show on full screen.
    ReactionIntensityMedium         ///<Reactions show on medium screen.
};

/*! \enum InterpretLanguage
    \brief Interpret language
    Here are more detailed structural descriptions.
*/
enum InterpretLanguage
{
    InterpretLanguageOriginalAudio = -1,    ///<Off, use original audio.
    InterpretLanguageEnglish = 0,           ///<English
    InterpretLanguageChinese = 1,           ///<Chinese
    InterpretLanguageJapanese = 2,          ///<Japanese
    InterpretLanguageGerman = 3,            ///<German
    InterpretLanguageFrench = 4,            ///<French
    InterpretLanguageRussian = 5,           ///<Russian
    InterpretLanguagePortuguese = 6,        ///<Portuguese
    InterpretLanguageSpanish = 7,           ///<Spanish
    InterpretLanguageKorean = 8,            ///<Korean
};

/*! \enum UserType
    \brief Meeting user type.
    Here are more detailed structural descriptions.
 */
enum UserType
{
    UserTypeNormal = 0,       ///<Normal user
    UserTypeH323 = 1,         ///<H323 user
    UserTypePureCallIn = 2    ///<Pure phone call in.
};

/*! \enum BO_STATUS
    \brief BO  status
    Here are more detailed structural descriptions.
 */
enum BO_STATUS
{
    BO_STATUS_INVALID = 0,     ///<Invalid
    BO_STATUS_EDIT = 1,        ///<Edit & Assign
    BO_STATUS_STARTED = 2,     ///<BO is started
    BO_STATUS_STOPPING = 3,    ///<BO is stopping
    BO_STATUS_ENDED = 4        ///<BO is ended
};

/*! \enum BO_USER_STATUS
    \brief BO  user status
    Here are more detailed structural descriptions.
 */
enum BO_USER_STATUS
{
    BO_USER_STATUS_INVALID = -1,    ///<BO user status invalid.
    BO_USER_STATUS_IN_MASTER = 1,   ///<BO user status in master conf.
    BO_USER_STATUS_IN_BO = 2,       ///<BO user status in BO conf.
    BO_USER_STATUS_LEAVE = 3,       ///<BO user status leave
};

////////////////////////////////////////// IZoomRoomsService ///////////////////////////////////////

class IZoomRoomsService;
/// \brief Zoom Room information.
///
struct ZoomRoomInfo
{
    std::string roomName;       ///<Room name.
    std::string displayName;    ///<Room display name.
    std::string roomAddress;    ///<Room IP address.
    
    std::string roomID;                     ///<The IZoomRoomsService's ID
    IZoomRoomsService* worker = nullptr;    ///<The IZoomRoomsService's handler.
    bool canRetryToPair = false;            ///<TRUE indicates can retry to pair to the last paired room.
};

////////////////////////////////////////// IPreMeetingService ///////////////////////////////////////

/// \brief Legacy room system
///
struct LegacyRoomSystem
{
    std::string                 name;                                       ///<Room system name.
    std::string                 ip;                                         ///<Room system ip.
    std::string                 e164Num;                                    ///<Room system e164 number.
    LegacyRoomSystemType        roomSystemType = LegacyRoomSystemTypeNone;  ///<Room system type, see \link LegacyRoomSystemType \endlink enum.
    LegacyRoomSystemEncryptType encryptType = LegacyRoomSystemEncryptTypeNo;///<Room system encrypt type, see \link LegacyRoomSystemEncryptType \endlink enum.
    bool                        isPexipCVI = false;                         ///<TRUE indicates the room system is pexip CVI. This field is configured in web.
};

/// \brief Dial number
///
struct DialNumber
{
    std::string countryCode;    ///<Dial number country code.
    std::string phoneNumber;    ///<Dial number phone number.
};

/// \brief Third party meeting information
/// Only valid when service provider is not ThirdPartyMeetingServiceProviderInvalid.
struct ThirdPartyMeeting
{
    ThirdPartyMeetingServiceProvider serviceProvider = ThirdPartyMeetingServiceProviderInvalid;    ///<Service provider, see \link ThirdPartyMeetingServiceProvider \endlink enum.
    std::string             meetingNumber;          ///<Meeting number.
    std::string             sipAddress;             ///<SIP address.
    std::string             h323Address;            ///<H323 address.
    std::string             joinMeetingURL;         ///<Join meeting url.
    std::vector<DialNumber> dialNumbers;            ///<Dial numbers.
};

/// \brief Event scheduled by user info
///
struct EventScheduledByUserInfo
{
    std::string userID;             ///<User ID.
    std::string userName;           ///<User name.
    std::string userAvatarURL;      ///<User avatar URL.
};

/// \brief Meeting item
/// MeetingItem extends CalendarEvent function with more Zoom meeting information.
struct MeetingItem
{
    ZoomMeetingItemType zoomMeetingItemType = ZoomMeetingItemTypeDefault;    ///<Meeting item type.

    std::string meetingNumber;         ///<Meeting number
    std::string meetingName;           ///<Meeting name
    std::string hostName;              ///<Host name
    std::string startTime;             ///<Start time, for instance:"2017-03-15T11:30:00-07:00"
    std::string endTime;               ///<End time, for instance:"2017-03-15T11:30:00-07:00"
    int32_t     scheduledFrom = -1;    ///<Only for Zoom Meeting, for compatibility, use plain int here, see \link MeetingScheduleFrom \endlink enum.

    bool isPrivate = false;        ///<TRUE indicates this is a private event.
    bool isAllDayEvent = false;    ///<TRUE indicates this is a all day event.
    bool isCheckedIn = false;      ///<TRUE indicates this event has been checked in.

    std::string meetingDomain;         ///<Meeting domain

    bool isInstantMeeting = false;     ///<TRUE indicates this is a Instant Meeting

    ThirdPartyMeeting thirdPartyMeetingInfo;    ///<Third party meeting information.
    EventScheduledByUserInfo scheduledByInfo;   ///<Event scheduled by user info.
};

////////////////////////////////////////// ISettingService //////////////////////////////////////////

/*! \enum SmartCameraMask
    \brief Smart camera mask.
    Here are more detailed structural descriptions.
 */
enum SmartCameraMask
{
    SmartCameraManualMask = 1 << 0,         ///<Manual
    SmartCameraSingleFocusMask = 1 << 1,    ///<Speaker Focus
    SmartCameraGroupFocusMask = 1 << 2,     ///<Group Focus
    SmartCameraMultiFocusMask = 1 << 3,     ///<Multi Focus
    SmartCameraMultiStreamMask = 1 << 4,    ///<Smart Gallery
    SmartCameraDirectorMask = 1 << 5,       ///<Director
    SmartCameraPresenterFocusMask = 1 << 6  ///<Presenter focus
};

/*! \enum VirtualAudioDeviceType
    \brief Virtual audio device type
    Here are more detailed structural descriptions.
 */
enum VirtualAudioDeviceType
{
    VirtualAudioDeviceTypeUnknown = -1,         ///<Unknown, indicates the Device not a supported virtual audio device.
    VirtualAudioDeviceTypeMicrophone,           ///<Microphone, indicates self a virtual microphone device.
    VirtualAudioDeviceTypeSpeaker               ///<Speaker, indicates self a virtual speaker device.
};

/*! \enum VirtualAudioDeviceVendor
    \brief Virtual audio device vendors
    Here are more detailed structural descriptions.
 */
enum VirtualAudioDeviceVendor
{
    VirtualAudioDeviceVendorDante = 0          ///<Dante
};

/// \brief Zoom Room generic settings
///
struct GenericSettings
{
    bool isMyVideoHidden = false;              ///<TRUE indicates ZR self video is hidden locally.
    bool muteAvWhenMeetingBegins = false;      ///<TRUE indicates audio & video is muted when meeting begins.
    bool isVideoSharingOptimizable = false;    ///<TRUE indicates ZR video sharing is optimizable.
    bool isVideoSharingOptimized = false;      ///<TRUE indicates ZR video sharing is optimized.
    bool isSupportExpandConfSelfVideo = false; ///<TRUE indicates ZR supports expand conf self video.
    bool isConfSelfVideoExpanded = false;      ///<TRUE indicates ZR  conf self video is expanded.
};

/// \brief Zoom Room hardware status
/// Some TV and ZoomRooms Applicance have privacy mute buttons, sync disable status.
struct HardwareStatus
{
    bool isMicDisabled = false;       ///<TRUE indicates ZR microphone is disabled.
    bool isCameraDisabled = false;    ///<TRUE indicates ZR camera is disabled.
};

/// \brief Virtual audio device
///
struct VirtualAudioDevice
{
    VirtualAudioDeviceType type = VirtualAudioDeviceTypeUnknown;    ///<Virtual audio device type, see \link VirtualAudioDeviceType \endlink enum.
    int32_t                vendor = 0;                              ///<Vendor, see \link VirtualAudioDeviceVendor \endlink enum.
    int32_t                maxSelectedCount = 0;                    ///<Max selected count
};

/// \brief Device item interface.
/// Device describes speaker, microphone, camera.
struct Device
{
    std::string id;                             ///<Device ID
    std::string name;                           ///<Device name
    std::string alias;                          ///<Device alias
    std::string displayName;                    ///<Device display name.
    bool        isSelected = false;             ///<TRUE indicates device is selected.
    bool        manuallySelected = false;       ///<TRUE indicates device is manually selected.
    bool        combinedDevice = false;         ///<TRUE indicates device is combined device.
    int32_t     numberOfCombinedDevices = 0;    ///<Number of combined devices.
    int32_t     ptzComId = -1;                  ///<PTZ com ID
    bool        isSelectedAsMultiDevice = false;///<TRUE indicates device is selected as multi device.
    bool        selectedDirectorDevice = false; ///<TRUE indicates the device is selected for director device.
    bool        isSupportCalibration = false;   ///<TRUE indicates support calibration.
    bool        isStethoscopeMic = false;       ///<TRUE indicates device is a stethoscope microphone.

    VirtualAudioDevice virtualAudioDevice;      ///<Virtual audio device info.
};

////////////////////////////////////////// IMeetingService //////////////////////////////////////////

/// \brief Meeting information.
///
struct MeetingInfo
{
    std::string meetingID;                        ///<Meeting ID
    std::string meetingNumber;                    ///<Meeting number
    std::string meetingName;                      ///<Meeting name
    MeetingType meetingType = MeetingTypeNone;    ///<Meeting type, see \link MeetingType \endlink enum
    std::string meetingPassword;                  ///<Meeting password
    std::string numericPassword;                  ///<Numeric password
    std::string inviteEmailTitle;                 ///<Invite email title
    std::string inviteEmailContent;               ///<Invite email content
    std::string joinMeetingUrl;                   ///<Join meeting url

    bool isWebinar = false;        ///<TRUE indicates it is a webinar meeting.
    bool isWaitingRoom = false;    ///<TRUE indicate it is a waiting room meeting.
   
    MeetingEncryptionAlgorithm encryptionAlgorithm = MeetingEncryptionNone; ///<Meeting encryption algorithm type.

    int  myUserId = -1;                    ///<ZR's user ID in current meeting.
    bool isWebinarAttendee = false;        ///<TRUE indicates ZR is webinar attendee in a webinar meeting.
    bool isWebinarAttendeeCanTalk = false; ///<TRUE indicates ZR is webinar attendee and is allowed to talk in a webinar meeting.
    bool amIOriginalHost = false;          ///<TRUE indicates ZR is the original host of meeting.
    bool canPutOnHold = false;             ///<TRUE indicates meeting supports put on hold.
    bool isAllowHostAssignCCEditor = false;///<TRUE indicates meeting allows host assign CC editor.
    bool isPAC = false;                    ///<TRUE indicates meeting is Personal Audio Conference.
    bool isPACVideoForbidden = false;      ///<TRUE indicates PAC meeting video is forbidden.
    bool isPACShareForbidden = false;      ///<TRUE indicates PAC meeting share is forbidden.
    bool isGreenRoomEnabled = false;       ///<TRUE indicates webinar green room is enabled.
    bool isDebriefSessionEnabled = false;  ///<TRUE indicates webinar debrief session  is enabled.
    bool isPrivateModeMeeting = false;     ///<TRUE indicates the meeting is hosted on a private MMR (Meeting Management Router) infrastructure, not operated by Zoom.
    bool isSimuliveWebinar = false;                 ///<TRUE indicates the meeting is simulive webinar.
    bool isSimuliveWebinarSupportGoLive = false;    ///<TRUE indicates the simulive webinar supports go live.
    bool isSimuliveWebinarAllowGoLive = false;      ///<TRUE indicates the simulive webinar is allowed go live.
};

/// \brief Audio status
///
struct AudioStatus
{
    AudioType audioType = AudioTypeNone;    ///<Audio type
    bool      isMuted = false;              ///<TRUE indicates audio is muted. Otherwise not.
};

/// \brief Video status
///
struct VideoStatus
{
    bool hasSource = false;     ///<TRUE indicates user has video source.
    bool receiving = false;     ///<TRUE indicates user is receiving other's video stream from meeting.
    bool sending = false;       ///<TRUE indicates user is sending video stream to meeting.
    bool canControl = false;    ///<TRUE indicates user's video can be controlled.
};

/// \brief New join flow info for video preview.
///
struct NewJoinFlowInfo
{
    bool showVideo = false;         ///<TRUE indicates to show video control.
    bool videoOn = false;           ///<TRUE indicates video is on.
    bool canTurnOnVideo = false;    ///<TRUE indicates can control video.
    bool showAudio = false;         ///<TRUE indicates to show audio control.
    bool audioOn = false;           ///<TRUE indicates audio is on.
    bool canTurnOnAudio = false;    ///<TRUE indicates can control audio.
};

/// \brief Camera control status
///
struct CameraControlStatus
{
    bool canIRequestControl = false;    ///<Indicates whether can be requested camera control.
    bool amIControlling = false;        ///<Indicates whether am I controlling.
    bool canSwitchCamera = false;       ///<Indicates whether can switch camera.
    bool canMoveCamera = false;         ///<Indicates whether can move camera position.
    bool canZoomCamera = false;         ///<Indicates whether can zoom in or zoom out camera.
};

/// \brief Hand status
///Raise hand status.
struct HandStatus
{
    bool         handRaised = false;                ///<TRUE indicates hand is raising.
    double       timeStamp = 0.0;                   ///<Raise hand time stamp
    HandSkinTone skinTone = HandSkinToneUnknown;    ///<Hand skin tone
};

/// \brief Interpret language information
///
struct InterpretLanguageInfo
{
    int32_t     language = InterpretLanguageOriginalAudio;    ///<Language, use int32_t for compatibility, see \link InterpretLanguage \endlink enum.
    std::string languageID;                                   ///<Language ID.
    std::string displayName;                                  ///<Language display name.
    std::string iconContent;                                  ///<Language icon.
};

/// \brief Meeting participant breakout room status
///
struct BreakoutRoomStatus
{
    std::string    sessionBID;                              ///<BO session BID.
    BO_USER_STATUS userStatus = BO_USER_STATUS_INVALID;     ///<BO user status.
    bool           supportForceJoinLeave = false;           ///<BO support force join and leave
    bool           supportSelfChooseRoom = false;           ///<BO support self choose room.
    bool           supportCohostStartStopBO = false;        ///<BO support cohost start or stop BO
    bool           supportTGMoveToMainSession = false;      ///<BO support move to main session.
    bool           isValidBOUser = false;                   ///<Indicates a valid user
};

/// \brief Smart tag user
///
struct SmartTagUser
{
    int32_t         tagID = -1;     ///<Tag ID.
    std::string     tagName;        ///<Tag name.
    std::string     avatarUrl;      ///<Avatar URL.
    std::string     avatarData;     ///<The binary data of avatar image(screenshot of the user’s head).
    int32_t         bindNodeID = 0; ///<Bind user node ID.
    std::string     bindEmail;      ///<Bind email adress.
    std::string     bindJid;        ///<Bind JID.
    std::string     defaultName;    ///<Default name(e.g. "Speaker 1" or "Participant 1")
    int32_t         streamUserID = 0; ///<The user ID of the video stream source, valid for Smart Name Tag.
};

/// \brief Webinar BO status.
///
struct WebinarBOStatus
{
    int32_t        uniqueJoinIndex = 0;                    ///<User unique ID between different webinar breakout room
    std::string    assignedSessionBID;                     ///<Assigned BO session BID.
    std::string    joinedSessionBID;                       ///<Joined BO session BID.
    BO_USER_STATUS userStatus = BO_USER_STATUS_INVALID;    ///<BO user status.
};

/// \brief Audio control status.
///
struct AudioControlStatus
{
    bool canIRequestAudioControl = false;        ///<TRUE indicates I can request audio control.
    bool canBeRequestedAudioControl = false;     ///<TRUE indicates the user can be requested audio control.
    bool amIControllingAudio = false;            ///<TRUE indicates I am controlling the user's audio.
};

/// \brief Meeting participant
///
struct MeetingParticipant
{
    int32_t               userID = 0;                                 ///<User ID
    int32_t               parentUserID = 0;                           ///<Parent's user ID, 0 indicates is a parent
    std::string           userGUID;                                   ///<User guid
    UserType              userType = UserTypeNormal;                  ///<User type, see \link UserType \endlink enum.
    std::string           userName;                                   ///<User name
    std::string           pronouns;                                   ///<User pronouns
    std::string           avatarUrl;                                  ///<Avatar url
    bool                  isMySelf = false;                           ///<TRUE indicates user is self.
    bool                  isHost = false;                             ///<TRUE indicates user is host.
    bool                  isOriginalOrAlternativeHost = false;        ///<TRUE indicates is original or alternative host.
    bool                  isCohost = false;                           ///<TRUE indicates user is co-host.
    bool                  isGuest = false;                            ///<TRUE indicates user is a guest of host account.
    bool                  isViewOnlyUser = false;                     ///<TRUE indicates user is an attendee in a Webinar
    bool                  isViewOnlyUserCanTalk = false;              ///<TRUE indicates user as an attendee in a Webinar can talk.
    bool                  canRecord = false;                          ///<TRUE indicates user can record
    bool                  isRecording = false;                        ///<TRUE indicates user is recording
    bool                  recordingDisabled = false;                  ///<TRUE indicates user is disabled from recording
    bool                  isInSilentMode = false;                     ///<TRUE indicates user is in silent mode.
    bool                  isLeavingSilentMode = false;                ///<TRUE indicates user is leaving silent mode.
    AudioStatus           audioStatus;                                ///<User audio status
    VideoStatus           videoStatus;                                ///<User video status
    CameraControlStatus   cameraControlStatus;                        ///<User camera control status
    HandStatus            handStatus;                                 ///<User hand status
    std::string           reactionEmoji;                              ///<Reaction emoji, Unicode.
    ReactionFeedback      reactionFeedback = ReactionFeedbackNone;    ///<Reaction feedback, see \link ReactionFeedback \endlink enum.
    bool                  isInterpreter = false;                      ///<TRUE indicates user is interpreter.
    InterpretLanguageInfo activeInterpreterLanguage;                  ///<When user is interpreter, this is the active interpreter language info.
    bool                  isRemoteControlAdmin = false;               ///<TRUE indicates user is a remote control admin.
    bool                  isVirtualAssistant = false;                 ///<TRUE indicates user is a virtual assistant
    bool                  isCompanionModeUser = false;                ///<TRUE indicates user is a companion mode user.
    bool                  isCompanionZRUser = false;                  ///<TRUE indicates user is a Companion Zoom Room.
    BreakoutRoomStatus    breakoutRoomStatus;                         ///<User's breakout room status.
    bool                  canPinMultiVideo = false;                   ///<TRUE indicates user can pin multi video.
    bool                  isSupportGreenRoom = false;                 ///<TRUE indicates user support green room.
    bool                  isInGreenRoom = false;                      ///<TRUE indicates user is in green room.
    bool                  isJoiningGreenRoom = false;                 ///<TRUE indicates user is joining green room.
    std::vector<SmartTagUser> smartNameTags;                          ///<Smart name tags.
    std::vector<SmartTagUser> smartSpeakerTags;                       ///<Smart speaker tags.
    WebinarBOStatus       webinarBOStatus;                            ///<User's Webinar breakout room status.
    std::string           attendeeJid;                                ///<User's attendeeJid remains unchanged when switching between main session and sub-session.
    AudioControlStatus    audioControlStatus;                         ///<User's audio control status.
    bool                  isRTMPUser = false;                         ///<TRUE indicates user is RTMP (Real Time Messaging Protocol) user.
    bool                  isActiveRTMPUser = false;                   ///<TRUE indicates user is currrent actived RTMP user.
    bool                  isSimuliveUser = false;                     ///<TRUE indicates the user is simulive user.
    bool                  isActiveSimuliveUser = false;               ///<TRUE indicates the user is simulive user that is playing recording.
};

/// \brief Participant count info
///
struct ParticipantCount
{
    int32_t waitingRoomUserCount = 0;       ///<In waiting room user count.
    int32_t inMeetingUserCount = 0;         ///<In meeting user count.
    int32_t inBackstageUserCount = 0;       ///<In backstage user count.
};

////////////////////////////////////////// CameraControlHelper //////////////////////////////////////////

/*! \enum CameraControlType
    \brief Camera control type.
    Here are more detailed structural descriptions.
 */
enum CameraControlType
{
    CameraControlTypeStart = 0,    ///<Start camera control action, see \link CameraControlAction \endlink enum.
    CameraControlTypeContinue,     ///<Continue camera control action, see \link CameraControlAction \endlink enum.
    CameraControlTypeStop,         ///<Stop camera control action, see \link CameraControlAction \endlink enum.

    CameraControlTypeRequestedByRemote,             ///<Remote controller requested controlling
    CameraControlTypeGaveUpByRemote,                ///<Remote controller gave up controlling
    CameraControlTypeControlRequestToRemote,        ///<For later controlling of remote cameras
    CameraControlTypeControlGaveUpToRemote,         ///<For give controlling to remote camera
    CameraControlTypeControlOpenCtrlRemotePanel,    ///<Open remote camera control panel
    CameraControlTypeControlMovingSpeed             ///<For camera support adjust moving speed
};

/*! \enum CameraControlAction
    \brief Camera control action.
    Here are more detailed structural descriptions.
 */
enum CameraControlAction
{
    CameraControlActionMoveUp = 0,    ///<Move camera up
    CameraControlActionMoveDown,      ///<Move camera down
    CameraControlActionMoveLeft,      ///<Move camera left
    CameraControlActionMoveRight,     ///<Move camera right
    CameraControlActionZoomIn,        ///<Zoom in camera
    CameraControlActionZoomOut        ///<Zoom out camera
};

/*! \enum CalibrationCameraLayout
    \brief Calibration camera layout
    Here are more detailed structural descriptions.
 */
enum CalibrationCameraLayout
{
    CalibrationCameraLayoutUnknown = -1,                        ///<Unknown.
    CalibrationCameraLayoutLeftTopMidBottomRightTop = 0,        ///<Camera layout left-top, mid-bottom, right-top.
    CalibrationCameraLayoutLeftBottomMidTopRightBottom,         ///<Camera layout left-bottom, mid-top, right-bottom.
    CalibrationCameraLayoutLeftTopMidTopRightTop,               ///<Camera layout left-top. mid-top, right-top.
    CalibrationCameraLayoutLeftBottomMidBottomRightBottom       ///<Camera layout left-bottom, mid-bottom, right-bottom.
};

/*! \enum CalibrationResult
    \brief Calibration result
    Here are more detailed structural descriptions.
 */
enum CalibrationResult
{
    CalibrationResultInit = 0,              ///<Init
    CalibrationResultStop,                  ///<Stop
    CalibrationResultCalibrateSuccess,      ///<Success
    CalibrationResultCalibrateFailed,       ///<Failed
    CalibrationResultCameraPositionUpdate   ///<Camera position update
};

/*! \enum CalibrationFailedCode
    \brief Calibration failed code
    Here are more detailed structural descriptions.
 */
enum CalibrationFailedCode
{
    CalibrationFailedCodeUnknown = 0,       ///<Unknown
    CalibrationFailedCodeMoveError,         ///<Move error
    CalibrationFailedCodeCamError,          ///<Camera error
    CalibrationFailedCodeCountError         ///<Count error
};

/// \brief Camera preset info.
///
struct CameraPresetInfo
{
    int32_t                         defaultIndex = -1;           ///<Default preset index.
    uint32_t                        supportedPresetCount = 0;    ///<Device supported preset count.
    std::map<uint32_t, std::string> namedPresets;                ///<Saved named presets.
};

/// \brief Multi camera control info.
///
struct CameraControlInfo
{
    std::string      deviceID;                                        ///<Device ID
    bool             canCurrentCameraBeControlled = false;            ///<TRUE indicates current camera can be controlled.
    bool             isOwnVideoMirrored = false;                      ///<TRUE indicates own video is mirrored.
    int32_t          pantiltSpeedPercentage = -1;                     ///<Pantilt speed percentage.
    CameraPresetInfo cameraPresetInfo;                                ///<Camera preset info.
    SmartCameraMask  smartCameraCapability = SmartCameraManualMask;   ///<Smart camera capability, see \link SmartCameraMask \endlink enum.
    SmartCameraMask  smartCameraMode = SmartCameraManualMask;         ///<Smart camera mode, see \link SmartCameraMask \endlink enum.
};

/// \brief Companion ZR device info
///
struct CompanionZRDeviceInfo
{
    std::string                      companionZRID;                     ///<CZR ID.
    std::vector<Device>              cameraList;                        ///<CZR camera list
    CameraControlInfo                mainCameraInfo;                    ///<Main camera control info
    bool                             canSwitchCameraForVideo = false;   ///<TRUE indicates can switch camera in meeting.
    std::vector<Device>              switchableCameras;                 ///<CZR switchable camera list.
    std::vector<Device>              comDeviceList;                     ///<CZR com list.
    std::vector<CameraControlInfo>   multiCameraInfo;                   ///<CZR multi camera control infos.
    bool                             allowSmartGalleryAndMultiCameraParallel = false;   ///<TRUE indicates allow smart gallery and multi camera parallel.
    int32_t                          maxMultiCameraCount = 2;           ///<CZR max multi camera count.
    bool                             isCameraEnable = false;            ///<TRUE indicates CZR camera enabled.
    std::string                      companionZRName;                   ///<CZR Name
    bool                             isSupportRename = false;           ///<TRUE indicates Companion ZR can be renamed.
};

/// \brief Companion ZR device update notification.
///
struct CompanionZRDeviceUpdateNot
{
    std::vector<CompanionZRDeviceInfo> companionZRDeviceInfoList;   ///<CZR device list.
};

/// \brief Camera boundary info
///
struct CameraBoundaryInfo
{
    std::string  deviceID;  ///<Camera device ID.
    int32_t left = -1;      ///<The value of the left boundary.
    int32_t right = -1;     ///<The value of the right boundary.
    int32_t depth = -1;     ///<The value of the depth boundary.
};

/// \brief Director calibration info
///
struct DirectorCalibrationInfo
{
    CalibrationCameraLayout         cameraLayout = CalibrationCameraLayoutUnknown;  ///<Calibration camera layout,  see \link CalibrationCameraLayout \endlink enum.
    std::vector<CameraControlInfo>  directorCameraInfo;                             ///<Director camera infos.
};

/// \brief Director calibration notification.
///
struct DirectorCalibrationNot
{
    CalibrationResult              result = CalibrationResultInit;              ///<Calibration result, see \link CalibrationResult \endlink enum.
    DirectorCalibrationInfo        info;                                        ///<Calibration info.
    std::string                    failedReason;                                ///<Failed reason message.
    CalibrationFailedCode          failedCode = CalibrationFailedCodeUnknown;   ///<Failed reason code.
    bool                           boundaryInfoEnabled = false;                 ///<TRUE indicates boundary info is enable. Otherwise not.
    std::vector<CameraBoundaryInfo> cameraBoundaryInfos;                        ///<Camera boundary infos.
};

/*! \enum BO_STOP_COUNTDOWN
    \brief BO stop count down.
    Here are more detailed structural descriptions.
 */
enum BO_STOP_COUNTDOWN
{
    BO_STOP_NOT_COUNTDOWN,
    BO_STOP_COUNTDOWN_SECONDS_10,
    BO_STOP_COUNTDOWN_SECONDS_15,
    BO_STOP_COUNTDOWN_SECONDS_30,
    BO_STOP_COUNTDOWN_SECONDS_60,
    BO_STOP_COUNTDOWN_SECONDS_120,
};

/// \brief Breakout room info
///
struct BreakoutRoomInfo
{
    std::string sessionBID;     ///<Session BID.
    std::string sessionName;    ///<Session name.
};

/// \brief Gallery grid size
///
struct GalleryGridSize
{
    uint32_t maxRows = 0;       ///<Number of the grid rows.
    uint32_t maxColumns = 0;    ///<Number of the grid columns.
};

NS_ZRCSDK_END


#endif    //ZRC_ZRCSDKTYPES_H
