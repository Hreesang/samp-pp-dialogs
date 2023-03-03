// -
// External Packages
// -

#include <a_samp>
#include <crashdetect>

#define PP_SYNTAX_YIELD
#define PP_SYNTAX_AWAIT
#include <PawnPlus>

// -
// Internal Packages
// -

#include "pp_dialogs"


// -
// API
// -

static GetRandomSkin()
{
	new skinId = 74;
	while (skinId == 74) {
		skinId = random(311);
	}
	return skinId;
}

static TestAsyncDialog(playerid)
{
	new Task:t = ShowPlayerAsyncDialog(
		playerid,
		DIALOG_STYLE_MSGBOX,
		"Hello World | Async Dialog",
		"Through TestAsyncDialog, the World says \"Hello World.\"",
		"Hi!",
		"Ok!"
	);
	new responses[E_ASYNC_DIALOG];
	await_arr(responses) t;

	new string[144];
	format(
		string,
		sizeof(string),
		"Script: TestAsyncDialog - Response: %s",
		responses[E_ASYNC_DIALOG_RESPONSE] ? "true" : "false"
	);
	SendClientMessage(playerid, -1, string);
}

static TestAsyncDialogStr(playerid)
{
	new Task:t = ShowPlayerAsyncDialogStr(
		playerid,
		DIALOG_STYLE_MSGBOX,
		str_new_static("Hello World | Async Dialog Str"),
		str_new_static("Through TestAsyncDialogStr, the World says \"Hello World.\""),
		str_new_static("Hi!"),
		str_new_static("Ok!")
	);
	new responses[E_ASYNC_DIALOG];
	await_arr(responses) t;

	new string[144];
	format(
		string,
		sizeof(string),
		"Script: TestAsyncDialogStr - Response: %s",
		responses[E_ASYNC_DIALOG_RESPONSE] ? "true" : "false"
	);
	SendClientMessage(playerid, -1, string);
}


// -
// Internals
// -

main() 
{
	print("Copyright (c) 2023 - PawnPlus Dialogs by Hreesang");
}

public OnPlayerRequestClass(playerid, classid)
{
	SetSpawnInfo(playerid, NO_TEAM, GetRandomSkin(), 1.0, 2.0, 3.0, 4.0, 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, true);
	TogglePlayerSpectating(playerid, false);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	yield 1;
	
	TestAsyncDialogStr(playerid);
	TestAsyncDialog(playerid);
	return 1;
}
