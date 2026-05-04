using Xunit;
using PepperDash.ZoomRoom.Sdk;

namespace ZoomRoomSdk.Tests;

public class ZrcSdkEnumsTests
{
    // ── ConnectionState ───────────────────────────────────────────────────────

    [Fact] public void ConnectionState_None_IsMinusOne()        => Assert.Equal(-1, (int)ConnectionState.None);
    [Fact] public void ConnectionState_Established_IsZero()     => Assert.Equal(0,  (int)ConnectionState.Established);
    [Fact] public void ConnectionState_Connected_IsOne()        => Assert.Equal(1,  (int)ConnectionState.Connected);
    [Fact] public void ConnectionState_Disconnected_IsTwo()     => Assert.Equal(2,  (int)ConnectionState.Disconnected);

    // ── MeetingStatus ─────────────────────────────────────────────────────────

    [Fact] public void MeetingStatus_NotInMeeting_IsZero()          => Assert.Equal(0, (int)MeetingStatus.NotInMeeting);
    [Fact] public void MeetingStatus_ConnectingToMeeting_IsOne()    => Assert.Equal(1, (int)MeetingStatus.ConnectingToMeeting);
    [Fact] public void MeetingStatus_InMeeting_IsTwo()              => Assert.Equal(2, (int)MeetingStatus.InMeeting);
    [Fact] public void MeetingStatus_LoggedOut_IsThree()            => Assert.Equal(3, (int)MeetingStatus.LoggedOut);

    // ── RoomUnpairedReason ────────────────────────────────────────────────────

    [Fact] public void RoomUnpairedReason_TokenInvalid_IsOne()       => Assert.Equal(1, (int)RoomUnpairedReason.TokenInvalid);
    [Fact] public void RoomUnpairedReason_RefreshTokenFail_IsTwo()   => Assert.Equal(2, (int)RoomUnpairedReason.RefreshTokenFail);
    [Fact] public void RoomUnpairedReason_RemovedByAdmin_IsThree()   => Assert.Equal(3, (int)RoomUnpairedReason.RemovedByAdmin);
    [Fact] public void RoomUnpairedReason_BetaBuildExpired_IsFour()  => Assert.Equal(4, (int)RoomUnpairedReason.BetaBuildExpired);
    [Fact] public void RoomUnpairedReason_ZRInE2EMeeting_IsFive()    => Assert.Equal(5, (int)RoomUnpairedReason.ZRInE2EMeeting);

    // ── ConnectionState aligns with GetConnectionStateDescription ────────────

    [Fact]
    public void ConnectionState_ValuesAlignWithDescriptionMethod()
    {
        Assert.Equal("Connected and verified", ZrcSdkCodes.GetConnectionStateDescription((int)ConnectionState.Connected));
        Assert.Equal("Disconnected",           ZrcSdkCodes.GetConnectionStateDescription((int)ConnectionState.Disconnected));
        Assert.Equal("Established",            ZrcSdkCodes.GetConnectionStateDescription((int)ConnectionState.Established));
        Assert.Equal("None (invalid)",         ZrcSdkCodes.GetConnectionStateDescription((int)ConnectionState.None));
    }

    // ── MeetingStatus aligns with GetMeetingStatusDescription ────────────────

    [Fact]
    public void MeetingStatus_ValuesAlignWithDescriptionMethod()
    {
        Assert.Equal("Not in meeting",      ZrcSdkCodes.GetMeetingStatusDescription((int)MeetingStatus.NotInMeeting));
        Assert.Equal("In meeting",          ZrcSdkCodes.GetMeetingStatusDescription((int)MeetingStatus.InMeeting));
        Assert.Equal("Logged out",          ZrcSdkCodes.GetMeetingStatusDescription((int)MeetingStatus.LoggedOut));
    }
}
