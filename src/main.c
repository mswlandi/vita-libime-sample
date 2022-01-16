#include <psp2/sysmodule.h>
#include <psp2/display.h>
#include <psp2/common_dialog.h>
#include <psp2/libime.h>
#include <psp2/kernel/clib.h> 
#include <psp2/kernel/processmgr.h> 
#include <vita2d.h>

SceUInt32 libime_work[SCE_IME_WORK_BUFFER_SIZE / sizeof(SceInt32)];
SceWChar16 libime_out[SCE_IME_MAX_PREEDIT_LENGTH + SCE_IME_MAX_TEXT_LENGTH + 1];
char libime_initval[8] = { 1 };
SceImeCaret caret_rev;

void VITA_ImeEventHandler(void* arg, const SceImeEventData* e)
{
	switch (e->id) {
	case SCE_IME_EVENT_UPDATE_TEXT:
		if (e->param.text.caretIndex == 0) {
			// backspace
			sceImeSetText((SceWChar16*)libime_initval, 4);
		}
		else {
			// test for scancode from key
			// if not space, utf16_toutf8 to send text
			memset(&caret_rev, 0, sizeof(SceImeCaret));
			memset(libime_out, 0, ((SCE_IME_MAX_PREEDIT_LENGTH + SCE_IME_MAX_TEXT_LENGTH + 1) * sizeof(SceWChar16)));
			caret_rev.index = 1;
			sceImeSetCaret(&caret_rev);
			sceImeSetText((SceWChar16*)libime_initval, 4);
		}
		break;
	case SCE_IME_EVENT_PRESS_ENTER:
		// enter
		break;
	case SCE_IME_EVENT_PRESS_CLOSE:
		sceImeClose();
		break;
	}
}

int main(int argc, const char *argv[]) {

	sceSysmoduleLoadModule(SCE_SYSMODULE_IME);
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	memset(libime_out, 0, ((SCE_IME_MAX_PREEDIT_LENGTH + SCE_IME_MAX_TEXT_LENGTH + 1) * sizeof(SceWChar16)));

	SceImeParam param;
	SceInt32 res;
	sceImeParamInit(&param);
	param.supportedLanguages = SCE_IME_LANGUAGE_ENGLISH;
	param.languagesForced = SCE_FALSE;
	param.type = SCE_IME_TYPE_DEFAULT;
	param.option = SCE_IME_OPTION_NO_ASSISTANCE;
	param.inputTextBuffer = libime_out;
	param.maxTextLength = SCE_IME_MAX_TEXT_LENGTH;
	param.handler = VITA_ImeEventHandler;
	param.filter = NULL;
	param.initialText = (SceWChar16*)libime_initval;
	param.arg = NULL;
	param.work = libime_work;
	res = sceImeOpen(&param);

	if (res < 0) {
		sceKernelExitProcess(0);
		return 0;
	}

	while (1) {
		vita2d_start_drawing();
		vita2d_draw_fill_circle(200, 420, 100, RGBA8(0, 255, 0, 255));
		vita2d_end_drawing();

		sceImeUpdate();

		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();
	}
	
	vita2d_fini();

	sceKernelExitProcess(0);
	return 0;
}
