namespace PepperDash.ZoomRoom.Sdk;

/// <summary>
/// Translates ZRC SDK integer codes to human-readable strings.
/// Values sourced directly from ZRCSDKTypes.h and IMeetingService.h.
/// </summary>
public static class ZrcSdkCodes
{
    /// <summary>
    /// Translates a <c>ZRCSDKError</c> value to a description string.
    /// This is the general error code returned by most API calls.
    /// </summary>
    /// <param name="code">The error code returned by an SDK API call.</param>
    /// <returns>A human-readable description of the error.</returns>
    public static string GetErrorDescription(int code) => code switch
    {
        // General
        0   => "Success",
        1   => "Internal error",
        2   => "Service not initialized",
        3   => "No permission",
        4   => "Feature disabled",
        5   => "Zoom Room has no capability for this action",
        6   => "Invalid parameter",
        7   => "Evaluation build expired",
        8   => "API not supported in webinar",
        9   => "Wrong API usage",
        10  => "Already in this state",
        11  => "Not connected to Zoom Room",
        12  => "Host has no capability to respond",
        13  => "Host not in meeting",
        14  => "Cannot perform action (ZR in special mode)",
        15  => "API not supported in webinar backstage",
        16  => "Request already responded",
        17  => "Settings locked on web",
        // API context restrictions
        50  => "API not supported in normal meeting",
        51  => "API not supported in breakout room",
        52  => "API not supported in webinar breakout room",
        53  => "API not supported in webinar debrief session",
        54  => "API not supported while switching meeting",
        55  => "API not supported in simulive webinar",
        // Device
        100 => "Device does not exist",
        101 => "Microphone is disabled",
        102 => "Camera is disabled",
        103 => "No CEC screen",
        104 => "Microphone audio is muted",
        105 => "No local network audio device",
        106 => "Zoom Room is not an IoT device",
        107 => "Device does not support identify",
        108 => "Network audio device channel not found",
        109 => "Device not connected",
        110 => "Selected microphone is a stethoscope",
        111 => "Device has no capability",
        112 => "Microphone is troubleshooting",
        // Camera
        120 => "Camera cannot be controlled",
        121 => "Camera cannot move",
        122 => "Camera cannot zoom",
        123 => "Camera cannot switch",
        124 => "Camera cannot change pan/tilt speed",
        125 => "Camera does not support smart mode",
        126 => "Camera is not active",
        127 => "Camera control is not enabled",
        128 => "Camera is not sharing",
        129 => "Camera does not support preset",
        130 => "Speaker switching not supported",
        131 => "Companion ZR camera cannot be enabled",
        132 => "Director calibration not supported",
        133 => "Camera cannot director calibrate",
        134 => "Camera does not support calibration",
        135 => "Calibration in wrong order",
        136 => "Multi-camera not enabled",
        137 => "Max multi-camera count reached",
        138 => "Camera cannot be selected for multi-camera",
        139 => "Camera not found",
        140 => "Camera cannot set COM ID",
        141 => "Camera cannot set display name",
        142 => "Camera boundary not supported",
        143 => "Camera boundary configuration in wrong order",
        144 => "NDI camera is being shared and cannot be selected",
        // Virtual Background
        180 => "Cannot change virtual background — setting locked on web",
        181 => "Cannot change virtual background — in immersive cutout",
        182 => "Cannot set virtual background to none — force-enabled on web",
        183 => "Blur algorithm not supported",
        // Meeting
        200 => "Invalid meeting",
        201 => "Invalid meeting number",
        202 => "Not a third-party meeting",
        210 => "No unencrypted attendee in webinar",
        211 => "Not an AES-256-GCM encrypted meeting",
        212 => "Meeting has third-party audio",
        220 => "Already in a meeting",
        221 => "Not in a meeting",
        222 => "Not in a webinar",
        223 => "Not in a PSTN call-out meeting",
        224 => "Not in an end-to-end encrypted meeting",
        225 => "Not in an integration meeting",
        226 => "Not in webinar practice session",
        227 => "Meeting does not support waiting room",
        228 => "Meeting waiting room is locked",
        229 => "Cannot join via web client",
        230 => "In an end-to-end encrypted meeting",
        231 => "In waiting room",
        232 => "In webinar backstage debrief session",
        233 => "Self or user not in proper webinar position",
        234 => "Meeting chat not supported or enabled",
        235 => "Chat privilege control not supported",
        236 => "Incoming meeting call does not exist",
        237 => "Cannot join via SIP",
        238 => "Cannot join via phone",
        239 => "Current integration meeting not supported",
        // View Layout
        250 => "View layout following host order",
        251 => "Screen cannot be pinned",
        252 => "User video already pinned on screen",
        253 => "User already spotlighted",
        254 => "Meeting does not support multi-pin",
        255 => "No user pinned",
        256 => "Max pinned users reached",
        257 => "No user spotlighted",
        258 => "Max spotlighted users reached",
        260 => "Not enough users for spotlight",
        261 => "NDI not enabled",
        262 => "NDI source already pinned",
        263 => "Not in dynamic layout",
        264 => "No share source while in live feed",
        265 => "Cannot adjust screen — only one screen available",
        266 => "Cannot use main screen as confidence monitor",
        267 => "Screen position adjustment out of range",
        268 => "NDI index not persisted",
        279 => "Wrong number of spotlighted users",
        // Share
        300 => "Meeting share is disabled",
        301 => "Not sharing",
        302 => "Blackmagic not sharing",
        303 => "Blackmagic not viewing locally",
        304 => "Blackmagic share data not ready",
        305 => "Blackmagic local view data not ready",
        306 => "No camera is sharing",
        307 => "Pin share needs double-confirmation",
        308 => "Share audio control not supported on single screen",
        309 => "Share has no audio",
        310 => "Feature disabled while in multi-share",
        311 => "Screen cannot pin share",
        312 => "Cannot pin backstage share in webinar",
        313 => "NDI camera is selected and cannot be shared",
        314 => "Share cannot be pinned on confidence monitor screen",
        // Recording
        350 => "Recording not available in current meeting",
        351 => "Must confirm recording disclaimer before starting",
        352 => "Must set recording notification email before starting",
        353 => "No need to prompt recording disclaimer",
        // User
        400 => "Participant not found by user ID",
        401 => "User info not found by GUID",
        402 => "Multi-stream video user — action not supported",
        403 => "Phone call-in user — action not supported",
        404 => "User is a panelist",
        405 => "User is an attendee",
        406 => "User is in backstage",
        407 => "User cannot be allowed to record",
        408 => "User cannot be allowed to multi-pin",
        409 => "User cannot be spotlighted",
        410 => "User cannot be add-pinned",
        411 => "User cannot be add-spotlighted",
        412 => "User is a simulive user",
        413 => "User action not supported",
        // Caption / Transcription
        450 => "Closed caption not available",
        451 => "Closed caption disabled — use new LTT caption",
        452 => "Live transcription not enabled",
        453 => "Live transcription disabled — use new LTT caption",
        460 => "New LTT caption not enabled",
        461 => "New LTT translated caption not enabled",
        462 => "New LTT caption speaking language not available",
        463 => "New LTT caption translation language not available",
        // Breakout Room
        501 => "No permission to manage breakout room",
        502 => "Cannot create breakout room after it has started",
        503 => "Cannot edit breakout room after it has started",
        504 => "Breakout room count exceeds max",
        505 => "Web pre-assign breakout room not enabled",
        506 => "Broadcast message to breakout room is disabled",
        507 => "Cannot delete main session",
        508 => "No breakout rooms added",
        509 => "Breakout room not found by session ID",
        511 => "Breakout room not in progress",
        514 => "No permission to join unassigned breakout room",
        515 => "No permission to leave breakout room",
        516 => "Not in a breakout room",
        517 => "Not in main session",
        518 => "You are the breakout room manager",
        519 => "User not assigned to any breakout room",
        520 => "Breakout room is stopping",
        521 => "Breakout room at capacity",
        // BYOD
        550 => "BYOD mode not supported",
        551 => "BYOD camera control not supported",
        552 => "BYOD microphone control not supported",
        553 => "BYOD speaker control not supported",
        554 => "BYOD auto framing not supported",
        555 => "BYOD mode does not support manual operation",
        556 => "Not in correct BYOD status",
        557 => "BYOD HDMI share not supported or not ready",
        // Polling
        600 => "Polling session is inactive",
        601 => "Another device already has polling control",
        602 => "Not the polling in-charge device",
        603 => "Invalid polling question type",
        604 => "Polling question ID missing",
        605 => "Polling question index missing",
        606 => "Polling question answers missing",
        607 => "Polling answer ID missing",
        608 => "Polling answers count error",
        609 => "Single-choice question allows only one answer",
        610 => "Multi-choice question requires at least one answer",
        611 => "Polling sub-questions missing",
        612 => "Polling sub-question ID missing",
        613 => "Polling sub-question index missing",
        614 => "Polling sub-question answers missing",
        615 => "Polling sub-question answer ID missing",
        616 => "Polling sub-question answers count error",
        617 => "Polling sub-question allows only one answer",
        618 => "Text answer is empty",
        619 => "Text answer below minimum length",
        620 => "Text answer exceeds maximum length",
        621 => "Not all blanks filled in",
        622 => "ZR does not support showing poll overlay",
        623 => "Show poll overlay not enabled",
        // SIP
        650 => "Incoming call not in incoming calls list",
        651 => "Already have a SIP call",
        652 => "No SIP call currently",
        653 => "SIP call not in SIP calls list",
        654 => "SIP call not in correct state",
        655 => "Foreground call is not in-call",
        656 => "Not in SIP call activity",
        657 => "SIP service not available",
        658 => "Zoom Phone not available",
        659 => "Number of calls exceeds limit",
        660 => "Already have a merged call",
        661 => "This call cannot be merged",
        662 => "Call is transferring",
        663 => "Already have a warm-transfer call",
        664 => "Cannot transfer a merged call",
        665 => "Call has no related call ID",
        666 => "Related call does not exist",
        667 => "No foreground call",
        668 => "Call is an emergency call",
        669 => "Cannot change location permission during meeting",
        670 => "No emergency info",
        671 => "No unhold call",
        672 => "Not all SIP calls are in-call",
        // AI Companion
        700 => "AI Companion meeting summary disabled",
        701 => "AI Companion meeting query disabled",
        702 => "AI Companion: no privilege setting item",
        703 => "AI Companion: invalid privilege setting item",
        704 => "AI Companion: meeting summary notification email already set",
        705 => "AI Companion: meeting summary notification email not set",
        // Meeting Chat
        750 => "Meeting chat disabled",
        751 => "Not using new meeting chat experience",
        752 => "Must confirm CMC tip",
        753 => "No need to confirm CMC tip",
        754 => "Message content is empty",
        755 => "Message content exceeds max size",
        756 => "Chat thread not found",
        757 => "Chat message not found",
        758 => "Emoji not found",
        759 => "Cannot send message",
        760 => "Cannot reply to thread",
        761 => "Cannot delete message",
        762 => "Cannot edit message",
        763 => "Cannot react to message with emoji",
        764 => "Please use new chat APIs",
        765 => "Reply type must match thread type",
        766 => "Private chat user ID error",
        767 => "Can only react with selected emojis",
        // HWIO
        780 => "HWIO service unavailable",
        // Q&A
        800 => "Not the main controller",
        801 => "Not allowed to send question",
        802 => "Not allowed to send anonymous question",
        803 => "Not allowed to answer question",
        804 => "Not allowed to comment on question",
        805 => "Not allowed to upvote question",
        // Pro AV
        830 => "NDI and HWIO both disabled",
        // Reaction
        850 => "Reactions disabled",
        // Smart Tag
        870 => "Smart tag disabled",
        871 => "Smart tag not supported",
        _   => $"Unknown error ({code})"
    };

    /// <summary>
    /// Translates an <c>OnPairRoomResult</c> result code.
    /// These are <b>not</b> <c>ZRCSDKError</c> values — they are pair-specific codes returned by the Zoom server.
    /// </summary>
    /// <param name="code">The result code from the <see cref="ZrcSdk.PairRoomResult"/> event.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetPairRoomResultDescription(int code) => code switch
    {
        0        => "Success",
        100      => "Failed to connect to room",
        101      => "Room cannot verify connection",
        102      => "Timeout waiting for room verify response",
        5002     => "Stored credentials rejected by server — token may have expired or room was re-provisioned",
        30055016 => "Invalid activation code",
        _        => $"Pairing failed (server code {code})"
    };

    /// <summary>
    /// Translates a <c>MeetingStatus</c> value from the <see cref="ZrcSdk.MeetingStatus"/> event.
    /// </summary>
    /// <param name="code">The <see cref="SdkEventArgs.ErrorCode"/> from the <c>MeetingStatus</c> event.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetMeetingStatusDescription(int code) => code switch
    {
        0 => "Not in meeting",
        1 => "Connecting to meeting",
        2 => "In meeting",
        3 => "Logged out",
        _ => $"Unknown meeting status ({code})"
    };

    /// <summary>
    /// Translates a <c>ConnectionState</c> value from <see cref="ZrcSdk.GetConnectionState"/>.
    /// </summary>
    /// <param name="code">The connection state integer.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetConnectionStateDescription(int code) => code switch
    {
        -1 => "None (invalid)",
        0  => "Established",
        1  => "Connected and verified",
        2  => "Disconnected",
        _  => $"Unknown connection state ({code})"
    };

    /// <summary>
    /// Translates the exit reason portion of the <see cref="ZrcSdk.ExitMeeting"/> event's
    /// <see cref="SdkEventArgs.ErrorCode"/>. The reason is in the upper byte:
    /// <c>(errorCode >> 8) &amp; 0xFF</c>.
    /// </summary>
    /// <param name="reason">The reason byte extracted from the exit meeting event code.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetExitMeetingReasonDescription(int reason) => reason switch
    {
        0 => "Default",
        1 => "Joining breakout room",
        2 => "Leaving breakout room",
        3 => "Rejoining new meeting",
        _ => $"Unknown exit reason ({reason})"
    };

    /// <summary>
    /// Translates a <c>WaitingHostReason</c> value from the <see cref="ZrcSdk.ConnectionStateChanged"/> event
    /// when the Zoom Room is waiting for the host.
    /// </summary>
    /// <param name="code">The reason code.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetWaitingHostReasonDescription(int code) => code switch
    {
        0 => "Waiting for host to start meeting",
        1 => "Waiting for host to end another meeting",
        _ => $"Unknown waiting reason ({code})"
    };

    /// <summary>
    /// Translates a <c>RoomUnpairedReason</c> value fired when the device becomes unpaired.
    /// </summary>
    /// <param name="code">The unpaired reason code.</param>
    /// <returns>A human-readable description.</returns>
    public static string GetRoomUnpairedReasonDescription(int code) => code switch
    {
        1 => "Token is invalid",
        2 => "Internal token refresh failed",
        3 => "Device removed by admin",
        4 => "Evaluation build expired",
        5 => "Zoom Room is in E2E-encrypted meeting",
        _ => $"Unknown unpaired reason ({code})"
    };
}
