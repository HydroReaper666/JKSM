#include <3ds.h>

#include "ui.h"
#include "data.h"
#include "fs.h"
#include "util.h"

static ui::titleview *sysView;
static bool fldOpen = false;

static void fldCallback(void *)
{
    switch(ui::padKeysDown())
    {
        case KEY_B:
            fs::closeSaveArch();
            fldOpen = false;
            break;
    }
}

static void sysViewCallback(void *a)
{
    switch(ui::padKeysDown())
    {
        case KEY_A:
            {
                data::titleData *t = &data::sysDataTitles[sysView->getSelected()];
                if(fs::openArchive(*t, ARCHIVE_SYSTEM_SAVEDATA, false))
                {
                    util::createTitleDir(*t, ARCHIVE_SYSTEM_SAVEDATA);
                    std::u16string targetDir = util::createPath(*t, ARCHIVE_SYSTEM_SAVEDATA);
                    ui::fldInit(targetDir, fldCallback, NULL);
                    fldOpen = true;
                }
            }
            break;

        case KEY_CPAD_LEFT:
            ui::state = EXT;
            break;

        case KEY_CPAD_RIGHT:
            ui::state = BOS;
            break;
    }
}

void ui::sysInit(void *a)
{
    threadInfo *t = (threadInfo *)a;
    t->status->setStatus("Preparing System Save View...");
    sysView = new ui::titleview(data::sysDataTitles, sysViewCallback, NULL);
    t->finished = true;
}

void ui::sysExit()
{
    delete sysView;
}

void ui::sysUpdate()
{
    if(fldOpen)
        ui::fldUpdate();
    else
        sysView->update();
}

void ui::sysDrawTop()
{
    ui::drawUIBar(TITLE_TEXT + "- System Saves", ui::SCREEN_TOP, true);
    sysView->draw();
}

void ui::sysDrawBot()
{
    if(fldOpen)
        ui::fldDraw();

    ui::drawUIBar("", ui::SCREEN_BOT, false);
}
