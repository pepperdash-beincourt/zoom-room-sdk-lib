using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AllowUserRecording(IntPtr handle, int userID, int allow);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ResponseToRecordingRequest(IntPtr handle, int accept, int acceptAlways);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetRecordingRequestCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetMeetingRecordingInfoCallback(IntPtr handle, ZrcMeetingRecordingInfoCallbackDelegate? cb, IntPtr userData);

    [StructLayout(LayoutKind.Sequential)]
    private struct ZrcMeetingRecordingInfoNative
    {
        public int isMeetingBeingRecorded;
        public int canIRecord;
        public int amIRecording;
        public int isConnectingToCMR;
        public int isCMRPaused;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcMeetingRecordingInfoCallbackDelegate(IntPtr infoPtr, IntPtr userData);

    private SdkEventCallbackDelegate? _recordingStatusCallbackDelegate;
    private SdkEventCallbackDelegate? _recordingRequestCallbackDelegate;
    private ZrcMeetingRecordingInfoCallbackDelegate? _meetingRecordingInfoCallbackDelegate;

    /// <summary>Cloud recording status changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if recording is in progress.</summary>
    public event EventHandler<SdkEventArgs>? RecordingStatus;
    /// <summary>
    /// A participant requested permission to record. <see cref="SdkEventArgs.Message"/> is the requester's display
    /// name (empty for a cloud recording request); <see cref="SdkEventArgs.ErrorCode"/> is the <see cref="RecordingType"/>
    /// value. Answer with <see cref="ResponseToRecordingRequest"/>.
    /// </summary>
    public event EventHandler<SdkEventArgs>? RecordingRequest;
    /// <summary>Meeting recording info changed — includes whether this room can record. See <see cref="MeetingRecordingInfoEventArgs"/>.</summary>
    public event EventHandler<MeetingRecordingInfoEventArgs>? MeetingRecordingInfoChanged;

    partial void InitializeRecordingCallbacks()
    {
        _recordingStatusCallbackDelegate       = OnRecordingStatusCallback;
        _recordingRequestCallbackDelegate      = OnRecordingRequestCallback;
        _meetingRecordingInfoCallbackDelegate  = OnMeetingRecordingInfoCallback;
        ZrcSdk_SetRecordingStatusCallback(_handle,       _recordingStatusCallbackDelegate,      IntPtr.Zero);
        ZrcSdk_SetRecordingRequestCallback(_handle,      _recordingRequestCallbackDelegate,     IntPtr.Zero);
        ZrcSdk_SetMeetingRecordingInfoCallback(_handle,  _meetingRecordingInfoCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Starts cloud recording. Must be host or have recording permission.</summary>
    public bool StartRecording() { ThrowIfDisposed(); return ZrcSdk_StartRecording(_handle) == 0; }

    /// <summary>Stops the active cloud recording.</summary>
    public bool StopRecording() { ThrowIfDisposed(); return ZrcSdk_StopRecording(_handle) == 0; }

    /// <summary>Pauses the active cloud recording.</summary>
    public bool PauseRecording() { ThrowIfDisposed(); return ZrcSdk_PauseRecording(_handle) == 0; }

    /// <summary>Resumes a paused cloud recording.</summary>
    public bool ResumeRecording() { ThrowIfDisposed(); return ZrcSdk_ResumeRecording(_handle) == 0; }

    /// <summary>Grants or revokes local recording permission for a participant. Host only.</summary>
    public bool AllowUserRecording(int userID, bool allow)
    {
        ThrowIfDisposed();
        return ZrcSdk_AllowUserRecording(_handle, userID, allow ? 1 : 0) == 0;
    }

    /// <summary>Accepts or declines a recording request from a participant.</summary>
    /// <param name="accept">Whether to approve the request.</param>
    /// <param name="acceptAlways">Whether to always approve future requests from this user.</param>
    public bool ResponseToRecordingRequest(bool accept, bool acceptAlways = false)
    {
        ThrowIfDisposed();
        return ZrcSdk_ResponseToRecordingRequest(_handle, accept ? 1 : 0, acceptAlways ? 1 : 0) == 0;
    }

    private void OnRecordingStatusCallback(string message, int isRecording, IntPtr userData) =>
        RecordingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isRecording });

    private void OnRecordingRequestCallback(string senderName, int recordingType, IntPtr userData) =>
        RecordingRequest?.Invoke(this, new SdkEventArgs { Message = senderName, ErrorCode = recordingType });

    private void OnMeetingRecordingInfoCallback(IntPtr infoPtr, IntPtr userData)
    {
        if (infoPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcMeetingRecordingInfoNative>(infoPtr);
        MeetingRecordingInfoChanged?.Invoke(this, new MeetingRecordingInfoEventArgs
        {
            IsMeetingBeingRecorded = n.isMeetingBeingRecorded != 0,
            CanIRecord             = n.canIRecord != 0,
            AmIRecording           = n.amIRecording != 0,
            IsConnectingToCloud    = n.isConnectingToCMR != 0,
            IsCloudRecordingPaused = n.isCMRPaused != 0,
        });
    }
}

/// <summary>Event args for <see cref="ZrcSdk.MeetingRecordingInfoChanged"/>.</summary>
public sealed class MeetingRecordingInfoEventArgs : System.EventArgs
{
    /// <summary><see langword="true"/> if the meeting is being recorded.</summary>
    public bool IsMeetingBeingRecorded { get; init; }
    /// <summary><see langword="true"/> if this room is allowed to start recording.</summary>
    public bool CanIRecord             { get; init; }
    /// <summary><see langword="true"/> if this room is currently recording.</summary>
    public bool AmIRecording           { get; init; }
    /// <summary>Cloud recording is connecting.</summary>
    public bool IsConnectingToCloud    { get; init; }
    /// <summary>Cloud recording is paused.</summary>
    public bool IsCloudRecordingPaused { get; init; }
}
