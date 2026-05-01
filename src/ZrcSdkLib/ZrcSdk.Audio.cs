namespace PepperDash.Zoom.ZrcSdk;

public partial class ZrcSdk
{
    private SdkEventCallbackDelegate? _audioStatusCallbackDelegate;
    private SdkEventCallbackDelegate? _muteOnEntryCallbackDelegate;

    /// <summary>
    /// Fired when the local audio mute state changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if muted, 0 if unmuted.
    /// </summary>
    public event EventHandler<SdkEventArgs>? AudioStatus;

    /// <summary>
    /// Fired when the mute-on-entry meeting setting changes.
    /// <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled, 0 if disabled.
    /// </summary>
    public event EventHandler<SdkEventArgs>? MuteOnEntry;

    partial void InitializeAudioCallbacks()
    {
        _audioStatusCallbackDelegate  = OnAudioStatusCallback;
        _muteOnEntryCallbackDelegate  = OnMuteOnEntryCallback;

        ZrcSdk_SetAudioStatusCallback(_handle,   _audioStatusCallbackDelegate,  IntPtr.Zero);
        ZrcSdk_SetMuteOnEntryCallback(_handle,   _muteOnEntryCallbackDelegate,  IntPtr.Zero);
    }

    /// <summary>Sets the local audio mute state.</summary>
    /// <param name="mute"><see langword="true"/> to mute, <see langword="false"/> to unmute.</param>
    public bool SetAudioMute(bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetAudioMute(_handle, mute ? 1 : 0) == 0;
    }

    /// <summary>Starts or stops the local camera.</summary>
    /// <param name="start"><see langword="true"/> to start video, <see langword="false"/> to stop.</param>
    public bool SetVideoState(bool start)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetVideoState(_handle, start ? 1 : 0) == 0;
    }

    private void OnAudioStatusCallback(string message, int isMuted, IntPtr userData) =>
        AudioStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isMuted });

    private void OnMuteOnEntryCallback(string message, int enabled, IntPtr userData) =>
        MuteOnEntry?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });
}
