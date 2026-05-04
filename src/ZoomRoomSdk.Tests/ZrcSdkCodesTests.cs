using Xunit;
using PepperDash.ZoomRoom.Sdk;

namespace ZoomRoomSdk.Tests;

public class ZrcSdkCodesTests
{
    // ── GetErrorDescription ───────────────────────────────────────────────────

    [Theory]
    [InlineData(0,   "Success")]
    [InlineData(1,   "Internal error")]
    [InlineData(3,   "No permission")]
    [InlineData(6,   "Invalid parameter")]
    [InlineData(11,  "Not connected to Zoom Room")]
    [InlineData(50,  "API not supported in normal meeting")]
    [InlineData(100, "Device does not exist")]
    [InlineData(120, "Camera cannot be controlled")]
    [InlineData(300, "Meeting share is disabled")]
    [InlineData(400, "Participant not found by user ID")]
    [InlineData(409, "User cannot be spotlighted")]
    [InlineData(450, "Closed caption not available")]
    [InlineData(501, "No permission to manage breakout room")]
    [InlineData(600, "Polling session is inactive")]
    [InlineData(650, "Incoming call not in incoming calls list")]
    [InlineData(700, "AI Companion meeting summary disabled")]
    [InlineData(750, "Meeting chat disabled")]
    [InlineData(800, "Not the main controller")]
    [InlineData(850, "Reactions disabled")]
    public void GetErrorDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetErrorDescription(code));
    }

    [Theory]
    [InlineData(9999)]
    [InlineData(-1)]
    [InlineData(int.MaxValue)]
    public void GetErrorDescription_UnknownCode_ReturnsUnknownWithCode(int code)
    {
        var result = ZrcSdkCodes.GetErrorDescription(code);
        Assert.StartsWith("Unknown error (", result);
        Assert.Contains(code.ToString(), result);
    }

    // ── GetConnectionStateDescription ────────────────────────────────────────

    [Theory]
    [InlineData(-1, "None (invalid)")]
    [InlineData(0,  "Established")]
    [InlineData(1,  "Connected and verified")]
    [InlineData(2,  "Disconnected")]
    public void GetConnectionStateDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetConnectionStateDescription(code));
    }

    [Fact]
    public void GetConnectionStateDescription_UnknownCode_ReturnsUnknownWithCode()
    {
        var result = ZrcSdkCodes.GetConnectionStateDescription(99);
        Assert.StartsWith("Unknown connection state (", result);
        Assert.Contains("99", result);
    }

    // ── GetMeetingStatusDescription ───────────────────────────────────────────

    [Theory]
    [InlineData(0, "Not in meeting")]
    [InlineData(1, "Connecting to meeting")]
    [InlineData(2, "In meeting")]
    [InlineData(3, "Logged out")]
    public void GetMeetingStatusDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetMeetingStatusDescription(code));
    }

    [Fact]
    public void GetMeetingStatusDescription_UnknownCode_ReturnsUnknownWithCode()
    {
        var result = ZrcSdkCodes.GetMeetingStatusDescription(42);
        Assert.StartsWith("Unknown meeting status (", result);
        Assert.Contains("42", result);
    }

    // ── GetPairRoomResultDescription ──────────────────────────────────────────

    [Theory]
    [InlineData(0,        "Success")]
    [InlineData(100,      "Failed to connect to room")]
    [InlineData(101,      "Room cannot verify connection")]
    [InlineData(102,      "Timeout waiting for room verify response")]
    [InlineData(5002,     "Stored credentials rejected by server — token may have expired or room was re-provisioned")]
    [InlineData(30055016, "Invalid activation code")]
    public void GetPairRoomResultDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetPairRoomResultDescription(code));
    }

    [Fact]
    public void GetPairRoomResultDescription_UnknownCode_ReturnsFailedWithCode()
    {
        var result = ZrcSdkCodes.GetPairRoomResultDescription(9999);
        Assert.StartsWith("Pairing failed (server code ", result);
        Assert.Contains("9999", result);
    }

    // ── GetExitMeetingReasonDescription ──────────────────────────────────────

    [Theory]
    [InlineData(0, "Default")]
    [InlineData(1, "Joining breakout room")]
    [InlineData(2, "Leaving breakout room")]
    [InlineData(3, "Rejoining new meeting")]
    public void GetExitMeetingReasonDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetExitMeetingReasonDescription(code));
    }

    [Fact]
    public void GetExitMeetingReasonDescription_UnknownCode_ReturnsUnknownWithCode()
    {
        var result = ZrcSdkCodes.GetExitMeetingReasonDescription(99);
        Assert.StartsWith("Unknown exit reason (", result);
        Assert.Contains("99", result);
    }

    // ── GetWaitingHostReasonDescription ──────────────────────────────────────

    [Theory]
    [InlineData(0, "Waiting for host to start meeting")]
    [InlineData(1, "Waiting for host to end another meeting")]
    public void GetWaitingHostReasonDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetWaitingHostReasonDescription(code));
    }

    [Fact]
    public void GetWaitingHostReasonDescription_UnknownCode_ReturnsUnknownWithCode()
    {
        var result = ZrcSdkCodes.GetWaitingHostReasonDescription(99);
        Assert.StartsWith("Unknown waiting reason (", result);
        Assert.Contains("99", result);
    }

    // ── GetRoomUnpairedReasonDescription ─────────────────────────────────────

    [Theory]
    [InlineData(1, "Token is invalid")]
    [InlineData(2, "Internal token refresh failed")]
    [InlineData(3, "Device removed by admin")]
    [InlineData(4, "Evaluation build expired")]
    [InlineData(5, "Zoom Room is in E2E-encrypted meeting")]
    public void GetRoomUnpairedReasonDescription_KnownCode_ReturnsExpected(int code, string expected)
    {
        Assert.Equal(expected, ZrcSdkCodes.GetRoomUnpairedReasonDescription(code));
    }

    [Fact]
    public void GetRoomUnpairedReasonDescription_UnknownCode_ReturnsUnknownWithCode()
    {
        var result = ZrcSdkCodes.GetRoomUnpairedReasonDescription(99);
        Assert.StartsWith("Unknown unpaired reason (", result);
        Assert.Contains("99", result);
    }
}
