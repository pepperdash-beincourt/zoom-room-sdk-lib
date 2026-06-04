namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Event arguments for directory/phonebook contact list updates received from the Zoom Room.
/// Raised in response to a contact subscription (<see cref="ZrcSdk.SubscribeContacts"/>).
/// </summary>
public sealed class ContactListEventArgs : System.EventArgs
{
    /// <summary>
    /// The contact data for this update batch.
    /// </summary>
    public ContactInfo[] Contacts { get; init; } = Array.Empty<ContactInfo>();
}
