using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    // ── P/Invoke: ZrcMeetingItem flat struct (must mirror ZrcSdkWrapper_C.h exactly) ──

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcMeetingItemNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string meetingNumber;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string meetingName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string hostName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string startTime;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]  public string endTime;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string meetingDomain;
        public int scheduledFrom;
        public int isPrivate;
        public int isAllDayEvent;
        public int isCheckedIn;
        public int isInstantMeeting;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcMeetingListCallbackDelegate(
        int result,
        IntPtr meetingsPtr,
        int count,
        IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingListCallback(
        IntPtr handle, ZrcMeetingListCallbackDelegate? cb, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ListMeeting(IntPtr handle);

    // ── Delegates ──────────────────────────────────────────────────────────────

    private ZrcMeetingListCallbackDelegate? _meetingListCallbackDelegate;

    // ── Events ─────────────────────────────────────────────────────────────────

    /// <summary>
    /// Fired when the scheduled-meeting (bookings/schedule) list is received in response to
    /// <see cref="ListMeeting"/>.
    /// </summary>
    public event EventHandler<MeetingListEventArgs>? MeetingListChanged;

    // ── Initializer ────────────────────────────────────────────────────────────

    partial void InitializeMeetingListCallbacks()
    {
        _meetingListCallbackDelegate = OnMeetingListCallback;
        ZrcSdk_SetMeetingListCallback(_handle, _meetingListCallbackDelegate, IntPtr.Zero);
    }

    // ── Public API ─────────────────────────────────────────────────────────────

    /// <summary>
    /// Requests the current list of scheduled meetings (calendar bookings) from the Zoom Room's
    /// configured calendar. Results arrive via <see cref="MeetingListChanged"/>.
    /// </summary>
    /// <returns><see langword="true"/> if the SDK accepted the request.</returns>
    public bool ListMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_ListMeeting(_handle) == 0;
    }

    // ── Callback handlers ──────────────────────────────────────────────────────

    private void OnMeetingListCallback(int result, IntPtr meetingsPtr, int count, IntPtr userData)
    {
        MeetingListChanged?.Invoke(this, new MeetingListEventArgs
        {
            Result   = result,
            Meetings = MarshalMeetings(meetingsPtr, count),
        });
    }

    private static MeetingItemInfo[] MarshalMeetings(IntPtr ptr, int count) =>
        MarshalNativeArray<ZrcMeetingItemNative, MeetingItemInfo>(ptr, count, native => new MeetingItemInfo
        {
            MeetingNumber    = native.meetingNumber ?? string.Empty,
            MeetingName      = native.meetingName ?? string.Empty,
            HostName         = native.hostName ?? string.Empty,
            StartTime        = native.startTime ?? string.Empty,
            EndTime          = native.endTime ?? string.Empty,
            MeetingDomain    = native.meetingDomain ?? string.Empty,
            ScheduledFrom    = native.scheduledFrom,
            IsPrivate        = native.isPrivate != 0,
            IsAllDayEvent    = native.isAllDayEvent != 0,
            IsCheckedIn      = native.isCheckedIn != 0,
            IsInstantMeeting = native.isInstantMeeting != 0,
        });
}
