using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcBORoomNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string sessionBID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string sessionName;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcBORoomListCallbackDelegate(IntPtr roomsPtr, int count, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StartBreakoutRooms(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_StopBreakoutRooms(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_JoinBreakoutRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_LeaveBreakoutRoom(IntPtr handle, string? boRoomBID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_BroadcastMessageToBreakoutRooms(IntPtr handle, string message);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AskForHelpInBreakoutRoom(IntPtr handle);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBOStatusChangedCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetBORoomListCallback(IntPtr handle, ZrcBORoomListCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate?    _boStatusChangedCallbackDelegate;
    private ZrcBORoomListCallbackDelegate? _boRoomListCallbackDelegate;

    /// <summary>Breakout room status changed. <see cref="SdkEventArgs.ErrorCode"/> is the BO_STATUS enum value.</summary>
    public event EventHandler<SdkEventArgs>? BOStatusChanged;
    /// <summary>Breakout room list updated.</summary>
    public event EventHandler<BORoom[]>? BORoomListUpdated;

    partial void InitializeBreakoutRoomCallbacks()
    {
        _boStatusChangedCallbackDelegate = OnBOStatusChangedCallback;
        _boRoomListCallbackDelegate      = OnBORoomListCallback;
        ZrcSdk_SetBOStatusChangedCallback(_handle, _boStatusChangedCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetBORoomListCallback(_handle,      _boRoomListCallbackDelegate,      IntPtr.Zero);
    }

    /// <summary>Starts breakout rooms. Host only.</summary>
    public bool StartBreakoutRooms() { ThrowIfDisposed(); return ZrcSdk_StartBreakoutRooms(_handle) == 0; }

    /// <summary>Stops breakout rooms and returns all participants to the main session. Host only.</summary>
    public bool StopBreakoutRooms() { ThrowIfDisposed(); return ZrcSdk_StopBreakoutRooms(_handle) == 0; }

    /// <summary>Joins the assigned breakout room (attendee action).</summary>
    public bool JoinBreakoutRoom() { ThrowIfDisposed(); return ZrcSdk_JoinBreakoutRoom(_handle) == 0; }

    /// <summary>Leaves the current breakout room (assistant action).</summary>
    public bool LeaveBreakoutRoom(string? boRoomBID = null) { ThrowIfDisposed(); return ZrcSdk_LeaveBreakoutRoom(_handle, boRoomBID) == 0; }

    /// <summary>Broadcasts a message to all breakout rooms. Host only.</summary>
    public bool BroadcastMessageToBreakoutRooms(string message) { ThrowIfDisposed(); return ZrcSdk_BroadcastMessageToBreakoutRooms(_handle, message) == 0; }

    /// <summary>Asks the host for help from within a breakout room (attendee action).</summary>
    public bool AskForHelpInBreakoutRoom() { ThrowIfDisposed(); return ZrcSdk_AskForHelpInBreakoutRoom(_handle) == 0; }

    private void OnBOStatusChangedCallback(string message, int status, IntPtr userData) =>
        BOStatusChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = status });

    private void OnBORoomListCallback(IntPtr roomsPtr, int count, IntPtr userData)
    {
        var rooms = MarshalBORooms(roomsPtr, count);
        BORoomListUpdated?.Invoke(this, rooms);
    }

    private static BORoom[] MarshalBORooms(IntPtr ptr, int count)
    {
        if (ptr == IntPtr.Zero || count <= 0) return Array.Empty<BORoom>();
        var result = new BORoom[count];
        int stride = Marshal.SizeOf<ZrcBORoomNative>();
        for (int i = 0; i < count; i++)
        {
            var n = Marshal.PtrToStructure<ZrcBORoomNative>(ptr + i * stride);
            result[i] = new BORoom { SessionBID = n.sessionBID ?? string.Empty, SessionName = n.sessionName ?? string.Empty };
        }
        return result;
    }
}

/// <summary>Represents a breakout room available within the current meeting.</summary>
public class BORoom
{
    /// <summary>Unique breakout room session BID string assigned by the Zoom platform.</summary>
    public string SessionBID  { get; set; } = string.Empty;
    /// <summary>Display name of the breakout room as configured by the host.</summary>
    public string SessionName { get; set; } = string.Empty;
}
