#pragma once

/* Messgae type(the first protocol) */
enum MessageType
{
	TYPE_SIGNIN,				// Sign in
	TYPE_SIGNUP,				// Sign up
	TYPE_SIGNOUT,				// Sign out
	TYPE_USER,				// User info
	TYPE_MATCH,				// Match
	TYPE_FIGHT,				// Fight
	TYPE_CHAT,				// Chat
	TYPE_NONE
};

/* The second of protocol of message type for the first protocol */
enum SMessageTypeFlags_
{
	// For the Match message
	SMessageTypeFlags_UserToMatch_SUC,			// the user match another user successfully (for the user, not the matched user)
	SMessageTypeFlags_UserToMatch_FAI,			// the user match another user failure (for the user, not the matched user)
	SMessageTypeFlags_UserBeMatched,			// the user is requested to match
	SMessageTypeFlags_UserIsForMatching,			// the user is matching the another user
	SMessageTypeFlags_UserToOnline,				// the user's state from matching to online
	SMessageTypeFlags_RejectOrOvertime,			// the matched user reject to the match message or overtime
	SMessageTypeFlags_MatchedUserAgree,			// the matched user agree with the match message

	// For Signout message of TYPE_SIGNOUT
	SMessageTypeFlags_IsTheUserSingout,			// For the signout user
	SMessageTypeFlags_NIsTheUserSingout,			// For not the signout users

	// For the Fight message
	SMessageTypeFlags_FightChatMsg,				// Chat message of Fight module
	SMessageTypeFlags_FightChessPosMsg,			// Send chess position infos of Fight module
	SMessageTypeFlags_FightExitMsg,				// Exit message of Fight module
	SMessageTypeFlags_FightVictory,				// Victory
	SMessageTypeFlags_FightFailure,				// Failure
	SMessageTypeFlags_FightUserToOnline			// Bordcast to all online users
};


/* Message data length */
#define MAX_DATA_SIZE	1024					// max data size

/* Message package header */
typedef struct data_header
{
	MessageType			msgType;		// Message type of the first protocol
	SMessageTypeFlags_	smsgType;
	unsigned int		dataSize;			// The real message data length in message package
}DATA_HEADER;

/* Message package type */
typedef struct data_package
{
	DATA_HEADER dataHeader;					// Message package header
	char		data[MAX_DATA_SIZE];			// The real message data

	data_package& operator = (const data_package& _data)
	{
		if (this != &_data)
		{
			dataHeader = _data.dataHeader;
			memset(data, 0, sizeof(data));
			memcpy(data, _data.data, sizeof(data));
		}
		return *this;
	}
}DATA_PACKAGE;


/* Fill the message package */
static inline void FilldPackage(const MessageType _type, const char* _msg, DATA_PACKAGE& dPackage)
{
	dPackage.dataHeader.msgType	 = _type;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);

	memset(dPackage.data, 0, sizeof(dPackage.data));
	strcpy(dPackage.data, _msg);
}

/* Fill the message package */
static inline void FilldPackage(const MessageType _type, SMessageTypeFlags_ _stype, const char* _msg, DATA_PACKAGE& dPackage)
{
	dPackage.dataHeader.smsgType = _stype;

	FilldPackage(_type, _msg, dPackage);
}
