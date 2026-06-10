using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    // ── P/Invoke: ZrcContact flat struct (must mirror ZrcSdkWrapper_C.h exactly) ──

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcContactNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string contactID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string screenName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string firstName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string lastName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string email;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string phoneNumber;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string sipPhoneNumber;
        public int presenceStatus;
        public int buddyType;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcContactListCallbackDelegate(
        IntPtr contactsPtr,
        int count,
        IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetContactListCallback(
        IntPtr handle, ZrcContactListCallbackDelegate? cb, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SubscribeContacts(IntPtr handle, int startIndex, int count, int isSearchSip);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_InviteAttendees(IntPtr handle, [In] string[] contactIDs, int count);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_MeetWithIMUsers(IntPtr handle, [In] string[] contactIDs, int count);

    // ── Delegates ──────────────────────────────────────────────────────────────

    private ZrcContactListCallbackDelegate? _contactListCallbackDelegate;

    // ── Events ─────────────────────────────────────────────────────────────────

    /// <summary>
    /// Fired when a batch of directory/phonebook contacts is received in response to
    /// <see cref="SubscribeContacts"/>.
    /// </summary>
    public event EventHandler<ContactListEventArgs>? ContactListChanged;

    // ── Initializer ────────────────────────────────────────────────────────────

    partial void InitializeContactsCallbacks()
    {
        _contactListCallbackDelegate = OnContactListCallback;
        ZrcSdk_SetContactListCallback(_handle, _contactListCallbackDelegate, IntPtr.Zero);
    }

    // ── Public API ─────────────────────────────────────────────────────────────

    /// <summary>
    /// Subscribes to a range of directory contacts. Results arrive via <see cref="ContactListChanged"/>.
    /// </summary>
    /// <param name="startIndex">Zero-based start index into the directory.</param>
    /// <param name="count">Number of contacts to request (the SDK suggests ~50 per page).</param>
    /// <param name="searchSip">When <see langword="true"/>, searches SIP contacts.</param>
    /// <returns><see langword="true"/> if the SDK accepted the request.</returns>
    public bool SubscribeContacts(int startIndex, int count, bool searchSip = false)
    {
        ThrowIfDisposed();
        // Native treats these as uint32_t; negatives would wrap to huge values. Fail predictably.
        if (startIndex < 0 || count < 0) return false;
        return ZrcSdk_SubscribeContacts(_handle, startIndex, count, searchSip ? 1 : 0) == 0;
    }

    /// <summary>
    /// Invites the given IM contacts (by contact ID) into the CURRENT meeting.
    /// </summary>
    /// <param name="contactIDs">Contact IDs to invite.</param>
    /// <returns><see langword="true"/> if the SDK accepted the request.</returns>
    public bool InviteAttendees(string[] contactIDs)
    {
        ThrowIfDisposed();
        var ids = FilterContactIds(contactIDs);
        if (ids.Length == 0) return false;
        return ZrcSdk_InviteAttendees(_handle, ids, ids.Length) == 0;
    }

    // Drop null/blank contact IDs before marshaling — the SDK expects valid strings.
    private static string[] FilterContactIds(string[] ids)
    {
        if (ids == null) return Array.Empty<string>();
        var list = new System.Collections.Generic.List<string>(ids.Length);
        foreach (var id in ids)
            if (!string.IsNullOrWhiteSpace(id)) list.Add(id);
        return list.ToArray();
    }

    /// <summary>
    /// Starts a NEW meeting with the given IM contacts (by contact ID).
    /// </summary>
    /// <param name="contactIDs">Contact IDs to meet with.</param>
    /// <returns><see langword="true"/> if the SDK accepted the request.</returns>
    public bool MeetWithIMUsers(string[] contactIDs)
    {
        ThrowIfDisposed();
        var ids = FilterContactIds(contactIDs);
        if (ids.Length == 0) return false;
        return ZrcSdk_MeetWithIMUsers(_handle, ids, ids.Length) == 0;
    }

    // ── Callback handlers ──────────────────────────────────────────────────────

    private void OnContactListCallback(IntPtr contactsPtr, int count, IntPtr userData)
    {
        ContactListChanged?.Invoke(this, new ContactListEventArgs
        {
            Contacts = MarshalContacts(contactsPtr, count),
        });
    }

    private static ContactInfo[] MarshalContacts(IntPtr ptr, int count) =>
        MarshalNativeArray<ZrcContactNative, ContactInfo>(ptr, count, native => new ContactInfo
        {
            ContactID      = native.contactID ?? string.Empty,
            ScreenName     = native.screenName ?? string.Empty,
            FirstName      = native.firstName ?? string.Empty,
            LastName       = native.lastName ?? string.Empty,
            Email          = native.email ?? string.Empty,
            PhoneNumber    = native.phoneNumber ?? string.Empty,
            SipPhoneNumber = native.sipPhoneNumber ?? string.Empty,
            PresenceStatus = native.presenceStatus,
            BuddyType      = native.buddyType,
        });
}
