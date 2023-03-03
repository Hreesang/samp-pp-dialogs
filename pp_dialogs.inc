// -
// Include Guards
// -

#if defined _INC_pp_dialogs
    #endinput
#endif
#define _INC_pp_dialogs


// -
// External Packages
// -

#if !defined _INC_PawnPlus && !defined _PawnPlus_included
    #error You need PawnPlus (github.com/IS4Code/PawnPlus) in order to use this library.
#endif


// -
// Pre-Hooks
// -
// This method was taken from https://github.com/oscar-broman/samp-weapon-config/issues/61#issuecomment-268567805
// Thanks, Y_Less. I only added a little parentheses on CHAIN_NEXT define '()'.
// -

#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}

#define CHAIN_NEXT(%0) @CO_%0()

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

static stock _PPD_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _PPD_IncludeStates() <_ALS : _ALS_go> {}
CHAIN_HOOK(PPD)
#undef CHAIN_ORDER
#define CHAIN_ORDER() CHAIN_NEXT(PPD)


// -
// Declarations
// -

#if !defined DIALOG_STYLE
    #define DIALOG_STYLE                        _
#endif

#if !defined PP_DIALOGS_ID
    #define PP_DIALOGS_ID                       (0x502B) // "P+" in ascii hex
#endif

#if !defined PP_DIALOGS_INPUT_LENGTH
    #define PP_DIALOGS_INPUT_LENGTH             128
#endif

enum _:E_ASYNC_DIALOG {
    E_ASYNC_DIALOG_RESPONSE,
    E_ASYNC_DIALOG_LISTITEM,
    E_ASYNC_DIALOG_INPUTTEXT[PP_DIALOGS_INPUT_LENGTH],
};

static stock Task:gs_dialogTask[MAX_PLAYERS] = {INVALID_TASK, ...};


// -
// API
// -

static stock PPD_ResetDialogs(playerid)
{
    if (gs_dialogTask[playerid] != INVALID_TASK) {
        task_delete(gs_dialogTask[playerid]);
        gs_dialogTask[playerid] = INVALID_TASK;
    }
    return 1;
}

// Calling native using this way as it wouldn't break any code hooked to that
// native in other modules.
forward _pp@showplayerdialogstr@call(playerid, dialogid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[]);
public _pp@showplayerdialogstr@call(playerid, dialogid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[])
{
    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
}

stock ShowPlayerDialogStr(playerid, dialogid, DIALOG_STYLE:style, ConstString:caption, ConstString:info, ConstString:button1, ConstString:button2 = ConstString:STRING_NULL)
{
    return CallLocalFunction(
        pawn_nameof(_pp@showplayerdialogstr@call),
        "iiiSSSS",
        playerid,
        dialogid,
        _:style,
        _:str_addr_const(caption),
        _:str_addr_const(info),
        _:str_addr_const(button1),
        _:str_addr_const(button2)
    );
}

stock Task:ShowPlayerAsyncDialog(playerid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[] = "")
{
    PPD_ResetDialogs(playerid);

    gs_dialogTask[playerid] = task_new();
    ShowPlayerDialog(playerid, PP_DIALOGS_ID, style, caption, info, button1, button2);

    return gs_dialogTask[playerid];
}

stock Task:ShowPlayerAsyncDialogStr(playerid, DIALOG_STYLE:style, ConstString:caption, ConstString:info, ConstString:button1, ConstString:button2 = ConstString:STRING_NULL)
{
    PPD_ResetDialogs(playerid);

    gs_dialogTask[playerid] = task_new();
    ShowPlayerDialogStr(playerid, PP_DIALOGS_ID, style, caption, info, button1, button2);

    return gs_dialogTask[playerid];
}


// -
// Internals
// -

public OnGameModeInit()
{
    state _ALS : _ALS_go;
    return PPD_OnGameModeInit();
}

public OnFilterScriptInit()
{
    state _ALS : _ALS_go;
    return PPD_OnFilterScriptInit();
}

public OnPlayerDisconnect(playerid, reason)
{
    PPD_ResetDialogs(playerid);
    return PPD_OnPlayerDiconnect(playerid, reason);
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == PP_DIALOGS_ID && gs_dialogTask[playerid] != INVALID_TASK) {
        // Store the current task
        new Task:t = gs_dialogTask[playerid];

        // Reset the player dialog task variable before clearing, so it
        // won't be reset when calling the PPD_ResetDialogs function.
        gs_dialogTask[playerid] = INVALID_TASK;
        PPD_ResetDialogs(playerid);
            
        new responses[E_ASYNC_DIALOG];
        responses[E_ASYNC_DIALOG_RESPONSE] = response;
        responses[E_ASYNC_DIALOG_LISTITEM] = listitem;
        strcat(responses[E_ASYNC_DIALOG_INPUTTEXT], inputtext, PP_DIALOGS_INPUT_LENGTH);
            
        task_set_result_ms_arr(t, responses, 0);
    }

    return PPD_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}


// -
// Hooks
// -

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit(%0) CHAIN_PUBLIC:PPD_OnGameModeInit(%0)
CHAIN_FORWARD:PPD_OnGameModeInit() = 1;

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit(%0) CHAIN_PUBLIC:PPD_OnFilterScriptInit(%0)
CHAIN_FORWARD:PPD_OnFilterScriptInit() = 1;

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:PPD_OnPlayerDiconnect(%0)
CHAIN_FORWARD:PPD_OnPlayerDiconnect(playerid, reason) = 1;

#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse(%0) CHAIN_PUBLIC:PPD_OnDialogResponse(%0)
CHAIN_FORWARD:PPD_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) = 0;


