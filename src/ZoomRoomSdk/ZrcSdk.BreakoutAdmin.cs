using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

/// <summary>Breakout room options (native BOOptions). Read-only fields are reported by the SDK and ignored on set.</summary>
public sealed class BOOptionsInfo
{
    public bool IsParticipantCanChooseRoom { get; set; }
    public bool IsParticipantCanReturnToMainSessionAtAnyTime { get; set; } = true;
    public bool IsAutoMoveAllAssignedParticipantsEnabled { get; set; }
    public bool IsBOTimerEnabled { get; set; }
    /// <summary>Seconds; used when <see cref="IsBOTimerEnabled"/>.</summary>
    public long BOTimerDuration { get; set; }
    public bool IsNotifyMeWhenTimeIsUp { get; set; }
    public BOStopCountdown CountdownSeconds { get; set; }
    /// <summary>Read-only.</summary>
    public BOStopCountdown DefaultCountDown { get; init; }
    /// <summary>Read-only.</summary>
    public long DefaultBOTimerDuration { get; init; }
    /// <summary>Read-only: the meeting was scheduled with web pre-assigned rooms.</summary>
    public bool IsPreAssignEnabled { get; init; }
    /// <summary>Read-only.</summary>
    public int MaxRoomCount { get; init; }
}

public partial class ZrcSdk
{
    // Mirrors ZrcBOOptions in ZrcSdkWrapper_C.h (64-bit fields first).
    [StructLayout(LayoutKind.Sequential)]
    private struct ZrcBOOptionsNative
    {
        public long boTimerDuration;
        public long defaultBOTimerDuration;
        public int  isParticipantCanChooseRoom;
        public int  isParticipantCanReturnToMainSessionAtAnyTime;
        public int  isAutoMoveAllAssignedParticipantsEnabled;
        public int  isBOTimerEnabled;
        public int  isNotifyMeWhenTimeIsUp;
        public int  countdownSeconds;
        public int  defaultCountDown;
        public int  isPreAssignEnabled;
        public int  maxRoomCount;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcBOOptionsCallbackDelegate(IntPtr optionsPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBOOptionsCallback(IntPtr handle, ZrcBOOptionsCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBOUserStatusCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBOTimerCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBOParticipantListCallback(IntPtr handle, ZrcParticipantListCallbackDelegate? cb, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_CreateBreakoutRooms(IntPtr handle, int count, int assignType);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AddBreakoutRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_DeleteBreakoutRoom(IntPtr handle, string sessionBID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_RenameBreakoutRoom(IntPtr handle, string sessionBID, string newName);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_AssignUsersToBreakoutRoom(IntPtr handle, string userGUIDsCsv, string sessionBID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetBOOptions(IntPtr handle, ref ZrcBOOptionsNative options);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetBOOptions(IntPtr handle, out ZrcBOOptionsNative options);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RequestBOOptions(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_MoveUserToBreakoutRoom(IntPtr handle, string userGUID, string sessionBID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_InviteBOUserReturnToMainSession(IntPtr handle, string userGUID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_IgnoreBOHelpRequest(IntPtr handle, string userGUID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinBreakoutRoomForHelp(IntPtr handle, string userGUID, string sessionBID, string sessionName);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinBreakoutRoomByBID(IntPtr handle, string sessionBID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RequestBreakoutRoomList(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RequestBreakoutRoomUserList(IntPtr handle);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ClaimHost(IntPtr handle, string hostKey);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AssignCohost(IntPtr handle, int userID, int assign);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_PromoteAttendeeToPanelist(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_DemotePanelistToAttendee(IntPtr handle, int userID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AllowWebinarAttendeeTalk(IntPtr handle, int userID, int allow);

    private ZrcBOOptionsCallbackDelegate?      _boOptionsCallbackDelegate;
    private SdkEventCallbackDelegate?          _boUserStatusCallbackDelegate;
    private SdkEventCallbackDelegate?          _boTimerCallbackDelegate;
    private ZrcParticipantListCallbackDelegate? _boParticipantListCallbackDelegate;

    /// <summary>Breakout options reported by the Zoom Room (on change, or after <see cref="RequestBOOptions"/>).</summary>
    public event EventHandler<BOOptionsInfo>? BOOptionsChanged;
    /// <summary>This room's own breakout status changed: <see cref="SdkEventArgs.ErrorCode"/> is <see cref="BOUserStatus"/>, Message the joined room BID.</summary>
    public event EventHandler<SdkEventArgs>? BOUserStatusChanged;
    /// <summary>Breakout timer tick (host): <see cref="SdkEventArgs.ErrorCode"/> is the remaining seconds.</summary>
    public event EventHandler<SdkEventArgs>? BOTimerTick;
    /// <summary>Breakout participant roster (host / co-host): who is assigned to and currently in which room. EventType 1 = full list.</summary>
    public event EventHandler<BOParticipantListEventArgs>? BOParticipantsUpdated;

    partial void InitializeBreakoutAdminCallbacks()
    {
        _boOptionsCallbackDelegate         = OnBOOptionsCallback;
        _boUserStatusCallbackDelegate      = OnBOUserStatusCallback;
        _boTimerCallbackDelegate           = OnBOTimerCallback;
        _boParticipantListCallbackDelegate = OnBOParticipantListCallback;
        ZrcSdk_SetBOOptionsCallback(_handle, _boOptionsCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetBOUserStatusCallback(_handle, _boUserStatusCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetBOTimerCallback(_handle, _boTimerCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetBOParticipantListCallback(_handle, _boParticipantListCallbackDelegate, IntPtr.Zero);
    }

    // ── Creator (host) ────────────────────────────────────────────────────────

    /// <summary>Creates <paramref name="count"/> breakout rooms with the given assignment mode. Host only, before the rooms are opened.</summary>
    public bool CreateBreakoutRooms(int count, BOAssignType assignType) { ThrowIfDisposed(); return ZrcSdk_CreateBreakoutRooms(_handle, count, (int)assignType) == 0; }
    public bool AddBreakoutRoom() { ThrowIfDisposed(); return ZrcSdk_AddBreakoutRoom(_handle) == 0; }
    public bool DeleteBreakoutRoom(string sessionBID) { ThrowIfDisposed(); return ZrcSdk_DeleteBreakoutRoom(_handle, sessionBID) == 0; }
    public bool RenameBreakoutRoom(string sessionBID, string newName) { ThrowIfDisposed(); return ZrcSdk_RenameBreakoutRoom(_handle, sessionBID, newName) == 0; }
    /// <summary>Assigns participants (by user GUID) to a room while rooms are being edited.</summary>
    public bool AssignUsersToBreakoutRoom(IEnumerable<string> userGUIDs, string sessionBID)
    {
        ThrowIfDisposed();
        return ZrcSdk_AssignUsersToBreakoutRoom(_handle, string.Join(",", userGUIDs), sessionBID) == 0;
    }

    public bool SetBOOptions(BOOptionsInfo options)
    {
        ThrowIfDisposed();
        var n = new ZrcBOOptionsNative
        {
            boTimerDuration = options.BOTimerDuration,
            isParticipantCanChooseRoom = options.IsParticipantCanChooseRoom ? 1 : 0,
            isParticipantCanReturnToMainSessionAtAnyTime = options.IsParticipantCanReturnToMainSessionAtAnyTime ? 1 : 0,
            isAutoMoveAllAssignedParticipantsEnabled = options.IsAutoMoveAllAssignedParticipantsEnabled ? 1 : 0,
            isBOTimerEnabled = options.IsBOTimerEnabled ? 1 : 0,
            isNotifyMeWhenTimeIsUp = options.IsNotifyMeWhenTimeIsUp ? 1 : 0,
            countdownSeconds = (int)options.CountdownSeconds,
        };
        return ZrcSdk_SetBOOptions(_handle, ref n) == 0;
    }

    public BOOptionsInfo? GetBOOptions()
    {
        ThrowIfDisposed();
        return ZrcSdk_GetBOOptions(_handle, out var n) == 0 ? ToOptions(n) : null;
    }

    /// <summary>Asks the SDK to report the current options on <see cref="BOOptionsChanged"/>.</summary>
    public bool RequestBOOptions() { ThrowIfDisposed(); return ZrcSdk_RequestBOOptions(_handle) == 0; }

    // ── Admin (host / co-host, rooms open) ────────────────────────────────────

    public bool MoveUserToBreakoutRoom(string userGUID, string sessionBID) { ThrowIfDisposed(); return ZrcSdk_MoveUserToBreakoutRoom(_handle, userGUID, sessionBID) == 0; }
    public bool InviteBOUserReturnToMainSession(string userGUID) { ThrowIfDisposed(); return ZrcSdk_InviteBOUserReturnToMainSession(_handle, userGUID) == 0; }
    public bool IgnoreBOHelpRequest(string userGUID) { ThrowIfDisposed(); return ZrcSdk_IgnoreBOHelpRequest(_handle, userGUID) == 0; }
    /// <summary>Joins the room of a participant who asked for help.</summary>
    public bool JoinBreakoutRoomForHelp(string userGUID, string sessionBID, string sessionName) { ThrowIfDisposed(); return ZrcSdk_JoinBreakoutRoomForHelp(_handle, userGUID, sessionBID, sessionName) == 0; }
    /// <summary>Joins a specific room (host / co-host visiting a room).</summary>
    public bool JoinBreakoutRoomByBID(string sessionBID) { ThrowIfDisposed(); return ZrcSdk_JoinBreakoutRoomByBID(_handle, sessionBID) == 0; }

    // ── Data ──────────────────────────────────────────────────────────────────

    /// <summary>Asks the SDK to report the room list on <see cref="BORoomListUpdated"/>.</summary>
    public bool RequestBreakoutRoomList() { ThrowIfDisposed(); return ZrcSdk_RequestBreakoutRoomList(_handle) == 0; }
    /// <summary>Asks the SDK to report the breakout roster on <see cref="BOParticipantsUpdated"/>.</summary>
    public bool RequestBreakoutRoomUserList() { ThrowIfDisposed(); return ZrcSdk_RequestBreakoutRoomUserList(_handle) == 0; }

    // ── Roles ─────────────────────────────────────────────────────────────────

    /// <summary>Claims host of the current meeting with the host key; the result arrives on OnClaimHostNotification (not surfaced) and as a host change.</summary>
    public bool ClaimHost(string hostKey) { ThrowIfDisposed(); return ZrcSdk_ClaimHost(_handle, hostKey) == 0; }
    /// <summary>Makes a participant co-host (assign = true) or removes co-host. Host only.</summary>
    public bool AssignCohost(int userID, bool assign) { ThrowIfDisposed(); return ZrcSdk_AssignCohost(_handle, userID, assign ? 1 : 0) == 0; }
    /// <summary>Webinar: promotes an attendee to panelist. Host / co-host.</summary>
    public bool PromoteAttendeeToPanelist(int userID) { ThrowIfDisposed(); return ZrcSdk_PromoteAttendeeToPanelist(_handle, userID) == 0; }
    /// <summary>Webinar: demotes a panelist to attendee. Host / co-host.</summary>
    public bool DemotePanelistToAttendee(int userID) { ThrowIfDisposed(); return ZrcSdk_DemotePanelistToAttendee(_handle, userID) == 0; }
    /// <summary>Webinar: allows or disallows an attendee to talk. Host / co-host.</summary>
    public bool AllowWebinarAttendeeTalk(int userID, bool allow) { ThrowIfDisposed(); return ZrcSdk_AllowWebinarAttendeeTalk(_handle, userID, allow ? 1 : 0) == 0; }

    // ── Callbacks ─────────────────────────────────────────────────────────────

    private static BOOptionsInfo ToOptions(ZrcBOOptionsNative n) => new BOOptionsInfo
    {
        IsParticipantCanChooseRoom = n.isParticipantCanChooseRoom != 0,
        IsParticipantCanReturnToMainSessionAtAnyTime = n.isParticipantCanReturnToMainSessionAtAnyTime != 0,
        IsAutoMoveAllAssignedParticipantsEnabled = n.isAutoMoveAllAssignedParticipantsEnabled != 0,
        IsBOTimerEnabled = n.isBOTimerEnabled != 0,
        BOTimerDuration = n.boTimerDuration,
        IsNotifyMeWhenTimeIsUp = n.isNotifyMeWhenTimeIsUp != 0,
        CountdownSeconds = (BOStopCountdown)n.countdownSeconds,
        DefaultCountDown = (BOStopCountdown)n.defaultCountDown,
        DefaultBOTimerDuration = n.defaultBOTimerDuration,
        IsPreAssignEnabled = n.isPreAssignEnabled != 0,
        MaxRoomCount = n.maxRoomCount,
    };

    private void OnBOOptionsCallback(IntPtr optionsPtr, IntPtr userData)
    {
        if (optionsPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcBOOptionsNative>(optionsPtr);
        BOOptionsChanged?.Invoke(this, ToOptions(n));
    }

    private void OnBOUserStatusCallback(string message, int status, IntPtr userData) =>
        BOUserStatusChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = status });

    private void OnBOTimerCallback(string message, int remaining, IntPtr userData) =>
        BOTimerTick?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = remaining });

    private void OnBOParticipantListCallback(IntPtr participantsPtr, int count, int eventType, int sessionType, IntPtr userData)
    {
        var participants = MarshalParticipants(participantsPtr, count);
        BOParticipantsUpdated?.Invoke(this, new BOParticipantListEventArgs
        {
            Participants = participants,
            TotalCount = count,
            EventType = eventType,
        });
    }
}

/// <summary>Breakout roster update from <see cref="ZrcSdk.BOParticipantsUpdated"/>.</summary>
public sealed class BOParticipantListEventArgs : System.EventArgs
{
    public ParticipantInfo[] Participants { get; init; } = Array.Empty<ParticipantInfo>();
    public int TotalCount { get; init; }
    /// <summary>0 = join, 1 = full list (replace), 2 = leave, 3 = update.</summary>
    public int EventType { get; init; }
}
