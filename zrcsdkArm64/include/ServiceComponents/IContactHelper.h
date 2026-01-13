/*!
 * \file IContactHelper.h
 * \since Added in ZRC SDK 6.0.0
 * \brief Contact interfaces.
 */
#ifndef ZRC_ICONTACTHELPER_H
#define ZRC_ICONTACTHELPER_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum IMAccountStatus
    \brief IM account status
    Here are more detailed structural descriptions.
 */
enum IMAccountStatus
{
    IMAccountStatusActive = 0,          ///<IM account status is active.
    IMAccountStatusDeactivated = 1,     ///<IM account status is deactivated.
    IMAccountStatusTerminate = 2        ///<IM account status is terminate.
};

/*! \enum IMBuddyType
    \brief IM buddy type
    Here are more detailed structural descriptions.
 */
enum IMBuddyType
{
    IMBuddyTypeNone = 0,                          ///<Init
    IMBuddyTypeDefault = 1 << 0,                  ///<Default
    IMBuddyTypeZoomRoom = 1 << 1,                 ///<Zoom Room
    IMBuddyTypeSharedGlobalDirectory = 1 << 2,    ///<Shared global directory, buddy from UCS.
    IMBuddyTypePersonalContacts = 1 << 3,         ///<Personal contact
    IMBuddyTypeZoomPhoneApplicance = 1 << 4,      ///<Zoom Phone Applicance
    IMBuddyTypeNormal = 1 << 5                    ///<Normal IM User
};

/*! \enum IMPresenceStatus
    \brief IM presence status
    Here are more detailed structural descriptions.
 */
enum IMPresenceStatus
{
    IMPresenceStatusNone,            ///<Invalid
    IMPresenceStatusAvailable,       ///<Online
    IMPresenceStatusUnAvailable,     ///<UnAvailable
    IMPresenceStatusInMeeting,       ///<In a Zoom meeting
    IMPresenceStatusBusy,            ///<Busy
    IMPresenceStatusDoNotDisturb,    ///<Do not disturb
    IMPresenceStatusAway,            ///<Away
    IMPresenceStatusPhoneCall,       ///<On a call
    IMPresenceStatusPresenting,      ///<Presenting
    IMPresenceStatusCalendar,        ///<In a calendar event
    IMPresenceStatusOutOfOffice      ///<Out of office
};

/// \brief Phone contact info.
/// Invalid if each filed is empty.
struct PhoneContact
{
    std::string label;          ///<Label.
    std::string countryCode;    ///<Country code.
    std::string phoneNumber;    ///<Phone number.
};

/// \brief Cloud PBX info
/// Invalid if each filed is empty.
struct CloudPBXInfo
{
    std::string                 extension;      ///<Extension number.
    std::string                 companyNumber;  ///<Company number.
    std::vector<std::string>    directNumbers;  ///<Direct numbers.
};

/// \brief Contact item.
///
struct Contact
{
    IMAccountStatus  accountStatus = IMAccountStatusActive;    ///<Account status, see \link IMAccountStatus \endlink enum.
    IMBuddyType      buddyType = IMBuddyTypeNone;              ///<Buddy type, see \link IMBuddyType \endlink enum.
    IMPresenceStatus presenceStatus = IMPresenceStatusNone;    ///<Presence status, see \link IMPresenceStatus \endlink enum.
    std::string      contactID;                                ///<Contact ID
    std::string      screenName;                               ///<Screen name
    std::string      firstName;                                ///<First name
    std::string      lastName;                                 ///<Last name
    std::string      phoneNumber;                              ///<Phone number
    std::string      email;                                    ///<Email
    std::string      pronouns;                                 ///<Pronouns
    std::string      company;                                  ///<Company
    std::string      department;                               ///<Department
    std::string      jobTitle;                                 ///<Job title
    std::string      avatarURL;                                ///<Avatar url
    std::string      location;                                 ///<Location
    std::string      signature;                                ///<Signature
    std::string      sipPhoneNumber;                           ///<Sip phone number
    CloudPBXInfo     cloudPBXInfo;                             ///<Cloud PBX info. Invalid if each filed is empty
    std::vector<PhoneContact> phoneInfos;                      ///<Phone contact infos
};

/// \brief Contact type info
///
struct ContactTypeInfo
{
    IMBuddyType buddyType = IMBuddyTypeNone;    ///<Buddy type
    uint32_t    count = 0;                      ///<Buddy count
};

/// \brief Dynamic contact list update information
///
struct DynamicContactListInfo
{
    bool                 isSearchSipContactResult = false;           ///<TRUE indicates this is a search sip contact result.
    uint32_t             startIndex = 0;                             ///<Start index
    uint32_t             numberOfContacts = 0;                       ///<Number of contacts
    std::string          filter;                                     ///<Search filter, will be an empty string if this is not a search result.
    uint32_t             totalNumberOfContactsInSearchResult = 0;    ///<Total number of contacts in search result
    std::vector<Contact> contacts;                                   ///<Contacts
    uint64_t             buddyTypeBits = 0;    ///<Bit value 1 indicates the buddy type being searched, see \link IMBuddyType \endlink enum.
};

/// \brief Favorite contact list.
///
struct FavoritesList
{
    std::vector<Contact>          favoriteContacts;             ///<Favorite contact list.
    std::vector<LegacyRoomSystem> favoriteLegacyRoomSystems;    ///<Favorite legacy room system list.
    std::vector<PhoneContact>     favoritePhoneContacts;        ///<Favorite phone contact list.
};

/// \brief Contact helper event callback
///
class IContactHelperSink
{
public:
    virtual ~IContactHelperSink() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief IM contacts update notification
     *
     * @param contacts Updated contacts.
     */
    virtual void OnImUpdateContactNotification(const std::vector<Contact>& contacts) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Dynamic contact basic info notification
     *
     * Use basic info to request needed contacts dynamically.
     *
     * @param typeInfos Contact type infos
     * @param numberOfContacts Contact number of IM contacts
     * @param numberOfZoomRooms Contact number of Zoom Rooms
     */
    virtual void OnDynamicContactBasicInfoNotification(const std::vector<ContactTypeInfo>& typeInfos, uint32_t numberOfContacts, uint32_t numberOfZoomRooms) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Dynamic contact list notification.
     *
     * @param info Dynamic contact list info
     */
    virtual void OnDynamicContactListNotification(const DynamicContactListInfo& info) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Update favorite list notification.
     *
     * @param list The favorite list.
     */
    virtual void OnUpdateFavoritesList(const FavoritesList& list) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Start receiving legacy room list notification.
     *
     * @param isSelfRooms TRUE indicates  is self rooms. Otherwise meeting's rooms.
     */
    virtual void OnStartReceivingLegacyRoomList(bool isSelfRooms) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Add legacy rooms notification.
     *
     * @param roomList The legacy room list.
     * @param isSelfRooms TRUE indicates  is self rooms. Otherwise meeting's rooms.
     */
    virtual void OnAddLegacyRooms(const std::vector<LegacyRoomSystem>& roomList, bool isSelfRooms) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Finishing receiving legacy room list notification.
     *
     * @param isSelfRooms TRUE indicates is self rooms. Otherwise meeting's rooms.
     */
    virtual void OnFinishReceivingLegacyRoomList(bool isSelfRooms) = 0;
};

/// \brief Contact helper interface
///
class IContactHelper
{
public:
    virtual ~IContactHelper() {}

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Register contact helper callback sink.
     *
     * @param pSink A pointer to the IContactHelperSink that receives the contact helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(IContactHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Deregister contact helper callback sink.
     *
     * @param pSink A pointer to the IContactHelperSink that receives the contact helper event callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(IContactHelperSink* pSink) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Subscribe contact by contact ID, you will be notified the changes after the subscription.
     *
     * @param contactIDs Specify the contactIDs to subscribe.
     * @param isSearchSipContact TRUE indicate to search sip contacts.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SubscribeByContactID(const std::vector<std::string>& contactIDs, bool isSearchSipContact) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Subscribe contact in range, you will be notified the changes after the subscription.
     *
     * @param startIndex Start index
     * @param count Subscribe contacts count, suggest 50.
     * @param isSearchSipContact TRUE indicate to search sip contacts.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError Subscribe(uint32_t startIndex, uint32_t count, bool isSearchSipContact) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Request contacts detail.
     *
     * @param contactIDs Specify the contactIDs to request.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestContactsDetail(const std::vector<std::string>& contactIDs) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Request contacts dynamically.
     *
     * @param startIndex Request contacts start index.
     * @param count Request contacts count
     * @param isSearchSipContact TRUE indicate to search sip contacts.
     * @param buddyTypeBits  Specify the buddy type bits to be requested, see \link IMBuddyType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RequestContactsDynamically(uint32_t startIndex, uint32_t count, bool isSearchSipContact, uint64_t buddyTypeBits) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Search contacts dynamically.
     *
     * @param startIndex Search contacts start index.
     * @param count Search contacts count
     * @param isSearchSipContact TRUE indicate to search sip contacts.
     * @param searchFilter Search contacts by filter.
     * @param buddyTypeBits Specify the buddy type bits to be searched, see \link IMBuddyType \endlink enum.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SearchContactsDynamically(uint32_t startIndex, uint32_t count, bool isSearchSipContact, const std::string& searchFilter, uint64_t buddyTypeBits) = 0;

    /**
     * @since 6.0.0
     * @zrminver 6.0.0
     *
     * @brief Query current legacy room list.
     *
     * @param[out] roomList Legacy room list.
     * @param isSelfRooms TRUE indicates to query self rooms. Otherwise query current meeting's rooms.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetLegacyRoomList(std::vector<LegacyRoomSystem>& roomList, bool isSelfRooms) = 0;
};

NS_ZRCSDK_END
#endif    //ZRC_ICONTACTHELPER_H
