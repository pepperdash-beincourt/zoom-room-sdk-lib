namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Managed representation of a Zoom Room directory/phonebook contact.
/// Mirrors the <c>ZrcContact</c> C struct with a lean set of the fields the host needs to
/// populate a directory entry and to dial/invite by contact ID.
/// </summary>
public sealed class ContactInfo
{
    /// <summary>Stable contact ID used for invite/dial-by-contact operations.</summary>
    public string ContactID      { get; init; } = string.Empty;
    /// <summary>Display (screen) name of the contact.</summary>
    public string ScreenName     { get; init; } = string.Empty;
    /// <summary>Contact's first name, if available.</summary>
    public string FirstName      { get; init; } = string.Empty;
    /// <summary>Contact's last name, if available.</summary>
    public string LastName       { get; init; } = string.Empty;
    /// <summary>Contact's email address, if available.</summary>
    public string Email          { get; init; } = string.Empty;
    /// <summary>Contact's phone number, if available.</summary>
    public string PhoneNumber    { get; init; } = string.Empty;
    /// <summary>Contact's SIP phone number, if available.</summary>
    public string SipPhoneNumber { get; init; } = string.Empty;
    /// <summary>Raw IMPresenceStatus enum value reported by the SDK.</summary>
    public int    PresenceStatus { get; init; }
    /// <summary>Raw IMBuddyType enum value reported by the SDK.</summary>
    public int    BuddyType      { get; init; }
}
