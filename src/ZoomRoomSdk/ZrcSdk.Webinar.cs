using System.Runtime.InteropServices;
using PepperDash.ZoomRoom.Sdk.EventArgs;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcWebinarAttendeeListCallbackDelegate(
        IntPtr attendeesPtr, int count, int total, int startIndex, int result,
        [MarshalAs(UnmanagedType.LPStr)] string keywords, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcWebinarCountsCallbackDelegate(int attendeeCount, int raisedHandCount, int panelistCount, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetWebinarAttendeeListCallback(IntPtr handle, ZrcWebinarAttendeeListCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetWebinarCountsCallback(IntPtr handle, ZrcWebinarCountsCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_ListWebinarAttendees(IntPtr handle, string keywords);

    private ZrcWebinarAttendeeListCallbackDelegate? _webinarAttendeeListCallbackDelegate;
    private ZrcWebinarCountsCallbackDelegate?       _webinarCountsCallbackDelegate;

    /// <summary>
    /// Webinar attendees (they are not part of the meeting roster): the answer to
    /// <see cref="ListWebinarAttendees"/>, and later pushes of the same list as it changes.
    /// </summary>
    public event EventHandler<WebinarAttendeeListEventArgs>? WebinarAttendeeListReceived;
    /// <summary>Webinar head counts changed (attendees, raised hands, panelists).</summary>
    public event EventHandler<WebinarCountsEventArgs>? WebinarCountsChanged;

    partial void InitializeWebinarCallbacks()
    {
        _webinarAttendeeListCallbackDelegate = OnWebinarAttendeeListCallback;
        _webinarCountsCallbackDelegate       = OnWebinarCountsCallback;
        ZrcSdk_SetWebinarAttendeeListCallback(_handle, _webinarAttendeeListCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetWebinarCountsCallback(_handle, _webinarCountsCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>
    /// Asks for webinar attendees: an empty <paramref name="keywords"/> returns the first 100, otherwise
    /// the attendees whose names match. Host / co-host. Answers on <see cref="WebinarAttendeeListReceived"/>.
    /// </summary>
    public bool ListWebinarAttendees(string keywords) { ThrowIfDisposed(); return ZrcSdk_ListWebinarAttendees(_handle, keywords ?? string.Empty) == 0; }

    private void OnWebinarAttendeeListCallback(IntPtr attendeesPtr, int count, int total, int startIndex, int result, string keywords, IntPtr userData) =>
        WebinarAttendeeListReceived?.Invoke(this, new WebinarAttendeeListEventArgs
        {
            Attendees  = MarshalParticipants(attendeesPtr, count),
            Total      = total,
            StartIndex = startIndex,
            Result     = result,
            Keywords   = keywords ?? string.Empty,
        });

    private void OnWebinarCountsCallback(int attendeeCount, int raisedHandCount, int panelistCount, IntPtr userData) =>
        WebinarCountsChanged?.Invoke(this, new WebinarCountsEventArgs
        {
            AttendeeCount   = attendeeCount,
            RaisedHandCount = raisedHandCount,
            PanelistCount   = panelistCount,
        });
}

/// <summary>A webinar attendee list from <see cref="ZrcSdk.WebinarAttendeeListReceived"/>.</summary>
public sealed class WebinarAttendeeListEventArgs : System.EventArgs
{
    public ParticipantInfo[] Attendees { get; init; } = Array.Empty<ParticipantInfo>();
    /// <summary>Attendees in the whole webinar; the list itself may hold fewer (the first 100, or a search result).</summary>
    public int Total { get; init; }
    /// <summary>0 = replace the local list; otherwise these entries continue a list already delivered.</summary>
    public int StartIndex { get; init; }
    /// <summary>0 on success.</summary>
    public int Result { get; init; }
    /// <summary>The search this list answers; empty for the unfiltered list.</summary>
    public string Keywords { get; init; } = string.Empty;
}

/// <summary>Webinar head counts from <see cref="ZrcSdk.WebinarCountsChanged"/>; -1 = not part of this notification.</summary>
public sealed class WebinarCountsEventArgs : System.EventArgs
{
    public int AttendeeCount { get; init; } = -1;
    public int RaisedHandCount { get; init; } = -1;
    public int PanelistCount { get; init; } = -1;
}
