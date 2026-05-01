namespace PepperDash.Zoom.ZrcSdk;

using System.Runtime.InteropServices;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _meetingStatusCallbackDelegate;
    private SdkEventCallbackDelegate? _startPmiResultCallbackDelegate;
    private SdkEventCallbackDelegate? _exitMeetingCallbackDelegate;
    private SdkEventCallbackDelegate? _meetingNeedsPasswordCallbackDelegate;
    private SdkEventCallbackDelegate? _meetingInviteCallbackDelegate;
    private SdkEventCallbackDelegate? _instantMeetingStartedCallbackDelegate;
    private SdkEventCallbackDelegate? _meetingLockStatusCallbackDelegate;

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_LockMeeting(IntPtr handle, int lockMeeting);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_EnableMeetingQA(IntPtr handle, int enable);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingLockStatusCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    /// <summary>Meeting lock status changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if locked.</summary>
    public event EventHandler<SdkEventArgs>? MeetingLockStatus;

    /// <summary>
    /// Fired when the meeting status changes.
    /// <see cref="SdkEventArgs.ErrorCode"/>: use <see cref="MeetingStatus"/> enum.
    /// </summary>
    public event EventHandler<SdkEventArgs>? MeetingStatus;

    /// <summary>
    /// Fired when a PMI or scheduled meeting starts.
    /// <see cref="SdkEventArgs.Message"/> is the meeting number; <see cref="SdkEventArgs.ErrorCode"/> is the result.
    /// </summary>
    public event EventHandler<SdkEventArgs>? StartPmiResult;

    /// <summary>
    /// Fired when a meeting ends.
    /// <see cref="SdkEventArgs.ErrorCode"/> encodes result (low byte) and reason (high byte).
    /// Decode: <c>result = e.ErrorCode &amp; 0xFF; reason = (e.ErrorCode >> 8) &amp; 0xFF</c>.
    /// </summary>
    public event EventHandler<SdkEventArgs>? ExitMeeting;

    /// <summary>
    /// Fired when the meeting requires a password.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if the previous attempt was wrong.
    /// Respond with <see cref="SendMeetingPassword"/> or <see cref="CancelEnteringMeetingPassword"/>.
    /// </summary>
    public event EventHandler<SdkEventArgs>? MeetingNeedsPassword;

    /// <summary>
    /// Fired when an incoming meeting invitation arrives.
    /// <see cref="SdkEventArgs.Message"/> contains the caller's name.
    /// </summary>
    public event EventHandler<SdkEventArgs>? MeetingInvite;

    /// <summary>
    /// Fired when an instant meeting starts.
    /// <see cref="SdkEventArgs.Message"/> is the meeting number.
    /// </summary>
    public event EventHandler<SdkEventArgs>? InstantMeetingStarted;

    partial void InitializeMeetingCallbacks()
    {
        _meetingLockStatusCallbackDelegate    = OnMeetingLockStatusCallback;
        ZrcSdk_SetMeetingLockStatusCallback(_handle, _meetingLockStatusCallbackDelegate, IntPtr.Zero);
        _meetingStatusCallbackDelegate        = OnMeetingStatusCallback;
        _startPmiResultCallbackDelegate       = OnStartPmiResultCallback;
        _exitMeetingCallbackDelegate          = OnExitMeetingCallback;
        _meetingNeedsPasswordCallbackDelegate = OnMeetingNeedsPasswordCallback;
        _meetingInviteCallbackDelegate        = OnMeetingInviteCallback;
        _instantMeetingStartedCallbackDelegate = OnInstantMeetingStartedCallback;

        ZrcSdk_SetMeetingStatusCallback(_handle,         _meetingStatusCallbackDelegate,        IntPtr.Zero);
        ZrcSdk_SetStartPmiResultCallback(_handle,        _startPmiResultCallbackDelegate,       IntPtr.Zero);
        ZrcSdk_SetExitMeetingCallback(_handle,           _exitMeetingCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetMeetingNeedsPasswordCallback(_handle,  _meetingNeedsPasswordCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetMeetingInviteCallback(_handle,         _meetingInviteCallbackDelegate,        IntPtr.Zero);
        ZrcSdk_SetInstantMeetingStartedCallback(_handle, _instantMeetingStartedCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Starts a meeting by meeting number or personal link name.</summary>
    public bool StartMeeting(string meetingNumber)
    {
        ThrowIfDisposed();
        return ZrcSdk_StartMeeting(_handle, meetingNumber) == 0;
    }

    /// <summary>Starts an instant (PMI) meeting. The meeting number arrives via <see cref="InstantMeetingStarted"/>.</summary>
    public bool StartInstantMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_StartInstantMeeting(_handle) == 0;
    }

    /// <summary>Starts a scheduled meeting using the host key.</summary>
    public bool StartMeetingWithHostKey(string hostKey)
    {
        ThrowIfDisposed();
        return ZrcSdk_StartMeetingWithHostKey(_handle, hostKey) == 0;
    }

    /// <summary>Joins a meeting by meeting number or personal link name.</summary>
    public bool JoinMeeting(string meetingNumber)
    {
        ThrowIfDisposed();
        return ZrcSdk_JoinMeeting(_handle, meetingNumber) == 0;
    }

    /// <summary>Joins a meeting using a Zoom invite URL.</summary>
    public bool JoinMeetingWithURL(string url)
    {
        ThrowIfDisposed();
        return ZrcSdk_JoinMeetingWithURL(_handle, url) == 0;
    }

    /// <summary>Ends the current meeting for all participants. Host only.</summary>
    public bool EndMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_EndMeeting(_handle) == 0;
    }

    /// <summary>Leaves the current meeting (self only).</summary>
    public bool LeaveMeeting()
    {
        ThrowIfDisposed();
        return ZrcSdk_LeaveMeeting(_handle) == 0;
    }

    /// <summary>Sends the meeting password when prompted by <see cref="MeetingNeedsPassword"/>.</summary>
    public bool SendMeetingPassword(string password)
    {
        ThrowIfDisposed();
        return ZrcSdk_SendMeetingPassword(_handle, password) == 0;
    }

    /// <summary>Cancels meeting password entry and aborts joining.</summary>
    public bool CancelEnteringMeetingPassword()
    {
        ThrowIfDisposed();
        return ZrcSdk_CancelEnteringMeetingPassword(_handle) == 0;
    }

    /// <summary>Locks or unlocks the meeting. Host only.</summary>
    public bool LockMeeting(bool lockMeeting) { ThrowIfDisposed(); return ZrcSdk_LockMeeting(_handle, lockMeeting ? 1 : 0) == 0; }

    /// <summary>Cancels waiting for the host and aborts joining.</summary>
    public bool CancelWaitingForHost()
    {
        ThrowIfDisposed();
        return ZrcSdk_CancelWaitingForHost(_handle) == 0;
    }

    private void OnMeetingLockStatusCallback(string message, int locked, IntPtr userData) =>
        MeetingLockStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = locked });

    private void OnMeetingStatusCallback(string message, int statusCode, IntPtr userData) =>
        MeetingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = statusCode });

    private void OnStartPmiResultCallback(string message, int result, IntPtr userData)
    {
        StartPmiResult?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
        if (result == 0)
            InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = result });
    }

    private void OnExitMeetingCallback(string message, int errorCode, IntPtr userData) =>
        ExitMeeting?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnMeetingNeedsPasswordCallback(string message, int wrongAndRetry, IntPtr userData) =>
        MeetingNeedsPassword?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = wrongAndRetry });

    private void OnMeetingInviteCallback(string message, int errorCode, IntPtr userData) =>
        MeetingInvite?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = errorCode });

    private void OnInstantMeetingStartedCallback(string meetingNumber, int result, IntPtr userData) =>
        InstantMeetingStarted?.Invoke(this, new SdkEventArgs { Message = meetingNumber, ErrorCode = result });
}
