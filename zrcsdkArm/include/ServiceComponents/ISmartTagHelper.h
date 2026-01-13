/*!
 * \file ISmartTagHelper.h
 * \since Added in ZRC SDK 6.3.5
 * \brief Smart tag  interfaces.
 *
 *
 * **Smart Speaker Tags**: There are two types:
 *      1. All Instance Smart Speaker Tag: Bind to smart speaker tag for participant in the meeting.
 *      2. Instance Only Smart Speaker Tag: Bind to smart speaker tag for individual LTT caption message.
 *
 * **Smart Name Tags**: There are two types:
 *      1. Unnamed Smart Name Tag, e.g.''Participant 1". This tag is visible only to yourself in the meeting.
 *      2. Named Smart Name Tag. This tag is visible to all participants in the meeting.
 *
 * **Suggested User List for Smart Tag Binding**:
 *      1. Companion mode users (Zoom client user pair to Zoom Rooms).
 *      2. User bound to the smart tag. Suggested binding smart speaker tag users are named smart name tags. Suggested binding smart name tag users are edited smart speaker tags.
 *      3. Meeting invitation attendees.
 *      4. Contact user. Call IContactHelper::RequestContactsDynamically to get contact list.
 */

#ifndef ISmartTagHelper_h
#define ISmartTagHelper_h

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum SmartTagType
    \brief Smart tag type.
    Here are more detailed structural descriptions.
 */
enum SmartTagType
{
    SmartTagTypeUnknown       = -1,     ///<Unknown.
    SmartTagTypeNameTag       = 0,      ///<Smart name tag.
    SmartTagTypeSpeakerTag    = 1,      ///<Smart speaker tag.
};

/*! \enum UnsupportSmartNameTagReason
    \brief Smart name tag unsuppoeted reason.
    Here are more detailed structural descriptions.
 */
enum UnsupportSmartNameTagReason
{
    UnsupportSmartNameTagReasonNone = -1,                   ///<None.
    UnsupportSmartNameTagReasonUnknown = 0,                 ///<Unknown.
    UnsupportSmartNameTagReasonHardware = 1,                ///<Hardware unsupported.
    UnsupportSmartNameTagReasonOptionDisabled = 2,          ///<Feature option disabled.
    UnsupportSmartNameTagReasonMultiFocus = 3,              ///<Multi focus mode.
    UnsupportSmartNameTagReasonSmartGallery = 4,            ///<Smart gallery mode.
    UnsupportSmartNameTagReasonIntelligentDirector = 5,     ///<Intelligent director mode.
};

/*! \enum UnsupportAutomaticReason
    \brief Unsupport automatic smart tag reason.
    Here are more detailed structural descriptions.
 */
enum UnsupportAutomaticReason
{
    UnsupportAutomaticReasonNone = -1,                   ///<None.
    UnsupportAutomaticReasonExceedInviteeMaximum = 0,    ///<The number of invitees has reached the maximum limit.
    UnsupportAutomaticReasonExceedEnrollMaximum = 1,     ///<The number of enrollments has reached the maximum limit.
    UnsupportAutomaticReasonWebOptionDisabled = 2,       ///<The web option "Automatic smart name tags for video/voice" is disabled.
};

/// \brief Smart Tag helper event callback.
///
class ISmartTagHelperSink
{
public:
    
    virtual ~ISmartTagHelperSink() {}
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Smart tag status updated notification.
     *
     * @param type Smart tag type, see \link SmartTagType \endlink enum.
     * @param enabled TRUE indicates that smart tag is enabled. Otherwise not.
     * @param isSupported TRUE indicates that smart tag is supported. Otherwise not.
     */
    virtual void OnSmartTagStatusUpdated(SmartTagType type, bool enabled, bool isSupported) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Smart tag suggested attendee updated notification.
     *
     * @param type Smart tag type, see \link SmartTagType \endlink enum.
     * @param attendees Suggested meeting invatation attendees, include companion mode users, bound smart tag users and invitation sttendees.
     */
    virtual void OnSmartTagSuggestedAttendeeUpdated(SmartTagType type, const std::vector<SmartTagUser>& attendees) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Unnamed smart name tag list updated notification.
     *
     * @param unnamedTagList Unnamed smart name tag list, see \link SmartTagUser \endlink struct.
     */
    virtual void OnUnnamedSmartNameTagListUpdated(const std::vector<SmartTagUser>& unnamedTagList) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Smart name tag avatar data updated (screenshot of the user’s head in the video) notification.
     *
     * @param tagID Smart name tag ID.
     * @param avatarData The binary data of avatar image.
     */
    virtual void OnNameTagAvatarUpdated(int32_t tagID, const std::string& avatarData) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief All instance smart speaker tag list updated notification.
     *
     * @param smartSpeakerTagList Smart speaker tag list,  see \link SmartTagUser \endlink struct.
     */
    virtual void OnAllInstanceSpeakerTagListUpdated(const std::vector<SmartTagUser>& smartSpeakerTagList) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Smart speaker tag avatar updated notification.
     *
     * @param jid Smart speaker tag bind JID.
     * @param avatarUrl Smart speaker tag bind JID user avatar URL.
     */
    virtual void OnSpeakerTagAvatarUpdated(const std::string& jid, const std::string& avatarUrl) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Smart name tag unsupported reason updated notification.
     *
     * @param reason Smart name tag unsupported reason, see \link UnsupportSmartNameTagReason \endlink enum.
     */
    virtual void OnNameTagUnsupportedReasonUpdated(UnsupportSmartNameTagReason reason) = 0;

    /**
     * @since 6.6.0
     * @zrminver 6.6.0
     *
     * @brief Automatic smart tag unsupported notification.
     *
     * @param tagType Smart tag type, see \link SmartTagType \endlink enum.
     * @param reason Automatic smart tag unsupported reason, see \link UnsupportAutomaticReason \endlink enum.
     * @param maxCount The maximum number of invitees or enrollments allowed, valid for reason is ExceedInviteeMaximum or ExceedEnrollMaximum.
     */
    virtual void OnAutomaticSmartTagUnsupported(SmartTagType tagType, UnsupportAutomaticReason reason, int32_t maxCount) = 0;
};

// \brief smart tag helper interface.
///
class ISmartTagHelper
{
public:
    virtual ~ISmartTagHelper() {}
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Register smart tag helper callback sink.
     *
     * @param pSink A pointer to the ISmartTagHelperSink that receives the smart tag  helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(ISmartTagHelperSink* pSink) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Deregister smart tag  helper callback sink.
     *
     * @param pSink A pointer to the ISmartTagHelperSink that receives the smart tag helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(ISmartTagHelperSink* pSink) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query all instance smart speaker tag list from participant list.
     *
     * @param[out] speakerTagList All instance smart speaker tag list, see \link SmartTagUser \endlink struct.
     * @param[in] userID Participant userID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetAllInstanceSpeakerTagList(int32_t userID, std::vector<SmartTagUser>& speakerTagList) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query named smart name tag list from participant list.
     *
     * @param[out] nameTagList Named smart name tag list, see \link SmartTagUser \endlink struct.
     * @param[in] userID Participant userID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetNamedNameTagList(int32_t userID, std::vector<SmartTagUser>& nameTagList)  = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query unnamed smart name tag list.
     *
     * @param[out] nameTagList  Unnamed smart name tag list, see \link SmartTagUser \endlink struct.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetUnnamedNameTagList(std::vector<SmartTagUser>& nameTagList) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query all instance speaker tag or named name tag from participant list.
     *
     * @param[out] tagUser Smart tag user, see \link SmartTagUser \endlink struct.
     * @param[in] type The smart tag type, see \link SmartTagType \endlink enum.
     * @param[in] userID Participant userID.
     * @param[in] tagID Smart tag ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetParticipantSmartTag(SmartTagType type, int32_t userID, int32_t tagID, SmartTagUser& tagUser) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Query smart speaker tag avatar by JID.
     *
     * @param[out] avatarUrl Smart speaker tag user avatar URL.
     * @param[in] jid Smart speaker tag user JID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetSpeakerTagAvatarByJid(const std::string& jid, std::string& avatarUrl) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Enable smart name/speaker tag.
     *
     * @param type The smart tag type, see \link SmartTagType \endlink enum.
     * @param enable TRUE indicates to enable smart tag. Otherwise not.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError EnableSmartTag(SmartTagType type, bool enable) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Request smart name/speaker tag suggested attendee.
     *
     * @param type The smart tag type, see \link SmartTagType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestSmartTagSuggestedAttendee(SmartTagType type) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Request smart name tag avatar data (screenshot of the user’s head).
     *
     * @param tagID Smart name tag ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestSmartNameTagAvatarData(int32_t tagID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Rename the edited smart speaker tag.
     *
     * @param editedTag Edited smart speaker tag, see \link SmartTagUser \endlink struct.
     * @param name Smart speaker tag name.
     * @param messageID LTT caption message ID.  If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameEditedSpeakerTag(const SmartTagUser& editedTag, const std::string& name, const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Rename the generic smart speaker tag.
     *
     * @param tagID Smart speaker tag ID.
     * @param name Smart speaker tag name.
     * @param messageID LTT caption message ID.  If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameGenericSpeakerTag(int32_t tagID, const std::string& name, const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Rename the smart name tag.
     *
     * @param tagID Smart name tag ID.
     * @param name Smart speaker tag name.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RenameNameTag(int32_t tagID, const std::string& name) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind the edited smart speaker tag to email.
     *
     * @param editedTag Edited smartspeaker tag, see \link SmartTagUser \endlink struct.
     * @param email Smart speaker tag bind email.
     * @param name Smart speaker tag bind name.
     * @param jid Smart speaker tag bind JID.
     * @param messageID LTT caption message ID. If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindEditedSpeakerTagToEmail(const SmartTagUser& editedTag, const std::string& email, const std::string& name,
                                                           const std::string& jid, const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind the generic smart speaker tag to email.
     *
     * @param tagID Smart speaker tag ID.
     * @param email Smart speaker tag bind email.
     * @param name Smart speaker tag bind name.
     * @param jid Smart speaker tag bind JID.
     * @param messageID LTT caption message ID. If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindGenericSpeakerTagToEmail(int32_t tagID, const std::string& email, const std::string& name, const std::string& jid,
                                                      const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind the smart name tag to email.
     *
     * @param tagID Smart name tag ID.
     * @param email Smart name tag bind email.
     * @param name Smart name tag bind name.
     * @param jid Smart name tag bind JID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindNameTagToEmail(int32_t tagID, const std::string& email, const std::string& name, const std::string& jid) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind the edited smart speaker tag to companion user.
     *
     * @param editedTag Edited smart speaker tag, see \link SmartTagUser \endlink struct.
     * @param userID Zoom Room companion user ID.
     * @param name Smart speaker tag bind name.
     * @param messageID LTT caption message ID. If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindEditedSpeakerTagToUserID(const SmartTagUser& editedTag, int32_t userID, const std::string& name,
                                                            const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind the generic smart speaker tag to companion user.
     *
     * @param tagID Generic smart speaker tag ID.
     * @param userID Zoom Room companion user ID.
     * @param name Smart speaker tag bind name.
     * @param messageID LTT caption message ID. If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindGenericSpeakerTagToUserID(int32_t tagID, int32_t userID, const std::string& name,
                                                       const std::string& messageID) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Bind smart name tag to companion user.
     *
     * @param tagID Smart name tag ID.
     * @param userID Zoom Room companion user ID.
     * @param name Smart name tag bind name.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError BindNameTagToUserID(int32_t tagID, int32_t userID, const std::string& name) = 0;

    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Unbind the edited smart speaker tag.
     *
     * @param editedTag Edited smart speaker tag, see \link SmartTagUser \endlink struct.
     * @param messageID LTT caption message ID. If provided, the instance only speaker tag is edited. If empty, all instance speaker  tag is edited.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnbindEditedSpeakerTag(const SmartTagUser& editedTag, const std::string& messageID) = 0;
    
    /**
     * @since 6.3.5
     * @zrminver 6.2.0
     *
     * @brief Unbind smart name tag.
     *
     * @param tagID Smart name tag ID.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError UnbindNameTag(int32_t tagID) = 0;
};

NS_ZRCSDK_END
#endif /* ISmartTagHelper_h */
