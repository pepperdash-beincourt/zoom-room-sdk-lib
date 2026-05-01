namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _recordingStatusCallbackDelegate;

    /// <summary>
    /// Fired when the cloud recording status changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if recording is in progress, 0 if stopped.
    /// </summary>
    public event EventHandler<SdkEventArgs>? RecordingStatus;

    partial void InitializeRecordingCallbacks()
    {
        _recordingStatusCallbackDelegate = OnRecordingStatusCallback;
        ZrcSdk_SetRecordingStatusCallback(_handle, _recordingStatusCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Starts cloud recording. Must be host or have recording permission.</summary>
    public bool StartRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_StartRecording(_handle) == 0;
    }

    /// <summary>Stops the active cloud recording.</summary>
    public bool StopRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_StopRecording(_handle) == 0;
    }

    /// <summary>Pauses the active cloud recording.</summary>
    public bool PauseRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_PauseRecording(_handle) == 0;
    }

    /// <summary>Resumes a paused cloud recording.</summary>
    public bool ResumeRecording()
    {
        ThrowIfDisposed();
        return ZrcSdk_ResumeRecording(_handle) == 0;
    }

    private void OnRecordingStatusCallback(string message, int isRecording, IntPtr userData) =>
        RecordingStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isRecording });
}
