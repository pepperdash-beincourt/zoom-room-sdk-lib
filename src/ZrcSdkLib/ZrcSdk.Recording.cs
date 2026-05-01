using System.Runtime.InteropServices;

namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AllowUserRecording(IntPtr handle, int userID, int allow);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_ResponseToRecordingRequest(IntPtr handle, int accept, int acceptAlways);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetRecordingRequestCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private SdkEventCallbackDelegate? _recordingStatusCallbackDelegate;
    private SdkEventCallbackDelegate? _recordingRequestCallbackDelegate;

    /// <summary>Cloud recording status changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if recording is in progress.</summary>
    public event EventHandler<SdkEventArgs>? RecordingStatus;
    /// <summary>A participant requested permission to record. <see cref="SdkEventArgs.ErrorCode"/> is the requesting userID; Message is the display name.</summary>
    public event EventHandler<SdkEventArgs>? RecordingRequest;

    partial void InitializeRecordingCallbacks()
    {
        _recordingStatusCallbackDelegate  = OnRecordingStatusCallback;
        _recordingRequestCallbackDelegate = OnRecordingRequestCallback;
        ZrcSdk_SetRecordingStatusCallback(_handle,  _recordingStatusCallbackDelegate,  IntPtr.Zero);
        ZrcSdk_SetRecordingRequestCallback(_handle, _recordingRequestCallbackDelegate, IntPtr.Zero);
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

    private void OnRecordingRequestCallback(string message, int userID, IntPtr userData) =>
        RecordingRequest?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = userID });
}
