using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

/// <summary>Third-party meeting service a Zoom Room can join (SDK ThirdPartyMeetingServiceProvider).</summary>
public enum ThirdPartyMeetingProvider
{
    Invalid        = 0,
    Webex          = 1,
    Skype          = 2,
    GoToMeeting    = 3,
    Teams          = 4,
    Bluejeans      = 5,
    GoogleHangouts = 6,
    Fuze           = 7,
    MetaWorkrooms  = 8,
    Tencent        = 9,
}

/// <summary>What an <see cref="IntegrationEventArgs"/> reports.</summary>
public enum IntegrationEventKind
{
    /// <summary>Code = result (0 = success), Value = provider.</summary>
    StartResult = 1,
    /// <summary>Code = <see cref="IntegrationMeetingState"/>, Value = provider, Fields = title, meeting ID, audio muted (1/0), video muted (1/0).</summary>
    MeetingInfo = 2,
    /// <summary>Code = error code, Fields[0] = message.</summary>
    Error = 3,
    /// <summary>Code = 1 when the room may join Teams meetings.</summary>
    SupportsTeams = 4,
    /// <summary>Code = 1 when the room may join Teams meetings ad hoc.</summary>
    SupportsTeamsAdhoc = 5,
    /// <summary>Code = 1 when the room may join Google Meet meetings.</summary>
    SupportsGoogleMeet = 6,
    /// <summary>Code = content share state (1 = active), Value = 1 when HDMI share is available.</summary>
    ContentShare = 7,
    /// <summary>Code = selected layout, Value = available layouts (bit mask).</summary>
    Layout = 8,
    /// <summary>The Zoom Room asks to join a third-party meeting. Fields = meeting name, meeting number.</summary>
    AskToJoin = 9,
}

/// <summary>State of the integration (third-party) meeting (SDK IntegrationMeetingState).</summary>
public enum IntegrationMeetingState
{
    None          = 0,
    Rejoining     = 1,
    Joining       = 2,
    WaitingRoom   = 3,
    Connected     = 4,
    Disconnecting = 5,
    Disconnected  = 6,
    NeedPassword  = 7,
}

/// <summary>An event from the Zoom Room's third-party (integration) meeting helper.</summary>
public sealed class IntegrationEventArgs : System.EventArgs
{
    public IntegrationEventKind Kind { get; init; }
    public int Code { get; init; }
    public int Value { get; init; }
    public string[] Fields { get; init; } = Array.Empty<string>();
}

/// <summary>Whether and how the Zoom Room may join a provider's meetings.</summary>
public sealed class InterOperabilityInfo
{
    public ThirdPartyMeetingProvider Provider { get; init; }
    public bool SupportJoinMeeting { get; init; }
    public bool SupportJoinWebClient { get; init; }
    public bool SupportSipJoin { get; init; }
    public bool SupportPhoneJoin { get; init; }
    public bool IsPexipEnabled { get; init; }
}

/// <summary>
/// Third-party (integration) meetings: the Zoom Room joins a Teams, Webex, Google Meet (or other)
/// meeting by its ID. Zoom Rooms 6.0+. Interop must be enabled for the room in the Zoom portal, and
/// the meeting's organizer must allow guests.
/// </summary>
public partial class ZrcSdk
{
    [StructLayout(LayoutKind.Sequential)]
    private struct ZrcInterOperabilityInfoNative
    {
        public int meetingType;
        public int supportJoinMeeting;
        public int supportJoinWebClient;
        public int supportSipJoin;
        public int supportPhoneJoin;
        public int isPexipEnabled;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcIntegrationEventCallbackDelegate(int kind, int code, int value, [MarshalAs(UnmanagedType.LPStr)] string? text, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetIntegrationEventCallback(IntPtr handle, ZrcIntegrationEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_JoinIntegrationMeeting(IntPtr handle, string meetingID, string? password, int provider);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LeaveIntegrationMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RejoinIntegrationMeeting(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_MuteIntegrationAudio(IntPtr handle, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StopIntegrationVideo(IntPtr handle, int stop);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GetInterOperabilityInfo(IntPtr handle, int provider, out ZrcInterOperabilityInfoNative outInfo);

    private ZrcIntegrationEventCallbackDelegate? _integrationEventCallbackDelegate;

    /// <summary>Raised for every third-party meeting event (start result, state, errors, support flags...).</summary>
    public event EventHandler<IntegrationEventArgs>? IntegrationEvent;

    partial void InitializeIntegrationCallbacks()
    {
        _integrationEventCallbackDelegate = OnIntegrationEventCallback;
        ZrcSdk_SetIntegrationEventCallback(_handle, _integrationEventCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Joins a third-party meeting by its meeting ID (and passcode, if it has one). Returns the SDK result code (0 = request accepted).</summary>
    public int JoinIntegrationMeeting(string meetingId, string? password, ThirdPartyMeetingProvider provider)
    {
        ThrowIfDisposed();
        if (string.IsNullOrWhiteSpace(meetingId)) return -1;
        return ZrcSdk_JoinIntegrationMeeting(_handle, meetingId.Trim(), password ?? string.Empty, (int)provider);
    }

    /// <summary>Leaves the current third-party meeting. Returns the SDK result code.</summary>
    public int LeaveIntegrationMeeting() { ThrowIfDisposed(); return ZrcSdk_LeaveIntegrationMeeting(_handle); }

    /// <summary>Rejoins the current third-party meeting. Returns the SDK result code.</summary>
    public int RejoinIntegrationMeeting() { ThrowIfDisposed(); return ZrcSdk_RejoinIntegrationMeeting(_handle); }

    /// <summary>Mutes or unmutes the room's audio in the third-party meeting. Returns the SDK result code.</summary>
    public int MuteIntegrationAudio(bool mute) { ThrowIfDisposed(); return ZrcSdk_MuteIntegrationAudio(_handle, mute ? 1 : 0); }

    /// <summary>Stops or starts the room's video in the third-party meeting. Returns the SDK result code.</summary>
    public int StopIntegrationVideo(bool stop) { ThrowIfDisposed(); return ZrcSdk_StopIntegrationVideo(_handle, stop ? 1 : 0); }

    /// <summary>Whether and how the room may join the provider's meetings, or null when the SDK can't say.</summary>
    public InterOperabilityInfo? GetInterOperabilityInfo(ThirdPartyMeetingProvider provider)
    {
        ThrowIfDisposed();
        if (ZrcSdk_GetInterOperabilityInfo(_handle, (int)provider, out var n) != 0) return null;
        return new InterOperabilityInfo
        {
            Provider             = (ThirdPartyMeetingProvider)n.meetingType,
            SupportJoinMeeting   = n.supportJoinMeeting != 0,
            SupportJoinWebClient = n.supportJoinWebClient != 0,
            SupportSipJoin       = n.supportSipJoin != 0,
            SupportPhoneJoin     = n.supportPhoneJoin != 0,
            IsPexipEnabled       = n.isPexipEnabled != 0,
        };
    }

    private void OnIntegrationEventCallback(int kind, int code, int value, string? text, IntPtr userData) =>
        IntegrationEvent?.Invoke(this, new IntegrationEventArgs
        {
            Kind   = (IntegrationEventKind)kind,
            Code   = code,
            Value  = value,
            Fields = string.IsNullOrEmpty(text) ? Array.Empty<string>() : text!.Split('\u001f'),
        });
}
