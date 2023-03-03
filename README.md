# samp-pp-dialogs

[![sampctl](https://img.shields.io/badge/sampctl-PawnPlus--Dialogs-2f2f2f.svg?style=for-the-badge)](https://github.com/Hreesang/samp-pp-dialogs)

## Installation

Simply install to your project:

```bash
sampctl install Hreesang/samp-pp-dialogs
```

Include in your code and begin using the library:

```pawn
#include <pp_dialogs>
```

It is recommended that you set a PawnPlus version explicitely on your pawn.json
(preferibly the latest) to avoid always downloading the latest one.

If you don't use sampctl, just download the `pp_dialogs.inc` include and
drop it to your `includes/` folder, and then download the PawnPlus plugin and
include [from here](https://github.com/IllidanS4/PawnPlus/releases).

While you're on it and if you don't use PawnPlus yet,
[you should check it out!](https://github.com/IllidanS4/PawnPlus/blob/master/README.md)

## Usage

This include provides one single function
```pawn
Task:ShowPlayerAsyncDialog(playerid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[] = "");
```

This will show the dialog and await for the response, which will pause the
current script's execution and return the yielded value to the last public
function (or 0 if it wasn't set). When it's responded to, the response details
will be inside the `responses[E_ASYNC_DIALOG]` array. If another
dialog gets shown while awaiting, the Task will be discarded with any following
code that was to be resumed.

For comfortability, it also provides support for PawnPlus strings:
```pawn
ShowPlayerDialogStr(playerid, dialogid, DIALOG_STYLE:style, ConstString:caption, ConstString:info, ConstString:button1, ConstString:button2 = ConstString:STRING_NULL);
Task:ShowPlayerAsyncDialogStr(playerid, DIALOG_STYLE:style, ConstString:caption, ConstString:info, ConstString:button1, ConstString:button2 = ConstString:STRING_NULL);
```

### Example
```pawn
static TestAsyncDialog(playerid)
{
	yield 1;
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
	yield 1;
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
```

## What's the Difference?

You might be wondering that because this library is quite similar with [async-dialogs](https://github.com/AGraber/samp-async-dialogs). Well, I have this library privately in my script because of some preference reasons. I don't find comfortable with the `async-dialogs` enumerator naming convention, and also there's a bug in there that messes up my script dialog IDs. I suspected that the bug happens because I have some libraries which hook the dialog functions and callback in my script, and the `async-dialogs` doesn't trigger the hooks because it directly calls the native.

## Thanks
* [IS4Code](https://github.com/IS4Code) for PawnPlus, which allows this and even more awesome stuff
* [AGraber](https://github.com/AGraber) for `async-dialogs`, most of the concept is from there, even the `README.md` is from there
* [Y-Less](https://github.com/Y-Less) for Pre-Hooks snippets
