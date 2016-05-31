#include "CPUArgumentWidget.h"
#include "ui_CPUArgumentWidget.h"
#include "Configuration.h"
#include "Bridge.h"

CPUArgumentWidget::CPUArgumentWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::CPUArgumentWidget),
    mTable(nullptr),
    mCurrentCallingConvention(-1),
    mStackOffset(0),
    mAllowUpdate(true)
{
    ui->setupUi(this);
    mTable = ui->table;
    setupTable();
    loadConfig();
    refreshData();

    mFollowDisasm = new QAction(this);
    connect(mFollowDisasm, SIGNAL(triggered()), this, SLOT(followDisasmSlot()));
    mFollowAddrDisasm = new QAction(this);
    connect(mFollowAddrDisasm, SIGNAL(triggered()), this, SLOT(followDisasmSlot()));
    mFollowDump = new QAction(this);
    connect(mFollowDump, SIGNAL(triggered()), this, SLOT(followDumpSlot()));
    mFollowAddrDump = new QAction(this);
    connect(mFollowAddrDump, SIGNAL(triggered()), this, SLOT(followDumpSlot()));
    mFollowStack = new QAction(this);
    connect(mFollowStack, SIGNAL(triggered()), this, SLOT(followStackSlot()));
    mFollowAddrStack = new QAction(this);
    connect(mFollowAddrStack, SIGNAL(triggered()), this, SLOT(followStackSlot()));

    connect(Bridge::getBridge(), SIGNAL(repaintTableView()), this, SLOT(refreshData()));
}

CPUArgumentWidget::~CPUArgumentWidget()
{
    delete ui;
}

void CPUArgumentWidget::disassembledAtSlot(dsint, dsint cip, bool, dsint)
{
    if(mCurrentCallingConvention == -1) //no calling conventions
    {
        mTable->setRowCount(0);
        mTable->reloadData();
        return;
    }

    BASIC_INSTRUCTION_INFO disasm;
    DbgDisasmFastAt(cip, &disasm);

    const auto & cur = mCallingConventions[mCurrentCallingConvention];
    mStackOffset = disasm.call ? 0 : cur.getCallOffset();
    if(ui->checkBoxLock->checkState() == Qt::PartiallyChecked) //Calls
    {
        mAllowUpdate = disasm.call;
        ui->spinArgCount->setEnabled(disasm.call);
        ui->comboCallingConvention->setEnabled(disasm.call);
    }
}

static QString stringFormatInline(const QString & format)
{
    if(!DbgFunctions()->StringFormatInline)
        return "";
    char result[MAX_SETTING_SIZE] = "";
    if(DbgFunctions()->StringFormatInline(format.toUtf8().constData(), MAX_SETTING_SIZE, result))
        return result;
    return "[Formatting Error]";

}

void CPUArgumentWidget::refreshData()
{
    if(!mAllowUpdate) //view is locked
        return;

    if(mCurrentCallingConvention == -1) //no calling conventions
    {
        mTable->setRowCount(0);
        mTable->reloadData();
        return;
    }

    const auto & cur = mCallingConventions[mCurrentCallingConvention];
    int argCountStruct = int(cur.arguments.size());
    int argCount = std::min(argCountStruct, ui->spinArgCount->value());
    int stackCount = std::max(0, ui->spinArgCount->value() - argCountStruct);
    mTable->setRowCount(argCount + stackCount);
    mArgumentValues.clear();

    for(int i = 0; i < argCount; i++)
    {
        const auto & curArg = cur.arguments[i];
        auto data = stringFormatInline(curArg.getFormat());
        auto text = defaultArgFieldFormat(defaultArgName(curArg.name, i + 1), data);
        mArgumentValues.push_back(DbgValFromString(curArg.getExpression().toUtf8().constData()));
        mTable->setCellContent(i, 0, text);
    }

    auto stackLocation = cur.getStackLocation();
    for(int i = 0; i < stackCount; i++)
    {
        duint argOffset = mStackOffset + i * sizeof(duint);
        QString expr = argOffset ? QString("%1+%2").arg(stackLocation).arg(ToHexString(argOffset)) : stackLocation;

        QString format = defaultArgFormat("", QString("[%1]").arg(expr));
        auto data = stringFormatInline(format);
        auto text = defaultArgFieldFormat(defaultArgName("", argCount + i + 1), data);
        mArgumentValues.push_back(DbgValFromString(expr.toUtf8().constData()));
        mTable->setCellContent(argCount + i, 0, text);
    }

    mTable->reloadData();
}

void CPUArgumentWidget::contextMenuSlot(QPoint pos)
{
    auto selection = mTable->getInitialSelection();
    if(int(mArgumentValues.size()) <= selection)
        return;
    auto value = mArgumentValues[selection];
    if(!DbgMemIsValidReadPtr(value))
        return;
    duint valueAddr;
    DbgMemRead(value, (unsigned char*)&valueAddr, sizeof(valueAddr));

    QMenu wMenu(this);
    auto valueText = ToHexString(value);
    auto valueAddrText = QString("[%1]").arg(valueText);
    auto configAction = [&](QAction * action, const QString & value, const QString & name)
    {
        action->setText(tr("Follow %1 in %2").arg(value).arg(name));
        action->setObjectName(value);
        wMenu.addAction(action);
    };
    auto inStackRange = [](duint addr)
    {
        auto csp = DbgValFromString("csp");
        duint size;
        auto base = DbgMemFindBaseAddr(csp, &size);
        return addr >= base && addr < base + size;
    };

    configAction(mFollowDisasm, valueText, tr("Disassembler"));
    configAction(mFollowDump, valueText, tr("Dump"));
    if(inStackRange(value))
        configAction(mFollowStack, valueText, tr("Stack"));
    if(DbgMemIsValidReadPtr(valueAddr))
    {
        configAction(mFollowAddrDisasm, valueAddrText, tr("Disassembler"));
        configAction(mFollowDump, valueAddrText, tr("Dump"));
        if(inStackRange(valueAddr))
            configAction(mFollowAddrStack, valueAddrText, tr("Stack"));
    }
    QMenu wCopyMenu(tr("&Copy"));
    mTable->setupCopyMenu(&wCopyMenu);
    if(wCopyMenu.actions().length())
    {
        wMenu.addSeparator();
        wMenu.addMenu(&wCopyMenu);
    }
    wMenu.exec(mTable->mapToGlobal(pos));
}

void CPUArgumentWidget::followDisasmSlot()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
        return;
    DbgCmdExec(QString("disasm \"%1\"").arg(action->objectName()).toUtf8().constData());
}

void CPUArgumentWidget::followDumpSlot()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
        return;
    DbgCmdExec(QString("dump \"%1\"").arg(action->objectName()).toUtf8().constData());
}

void CPUArgumentWidget::followStackSlot()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
        return;
    DbgCmdExec(QString("sdump \"%1\"").arg(action->objectName()).toUtf8().constData());
}

void CPUArgumentWidget::loadConfig()
{
#ifdef _WIN64
    CallingConvention x64(tr("Default (x64 fastcall)"), 1);
    x64.addArgument(Argument("", "", "rcx"));
    x64.addArgument(Argument("", "", "rdx"));
    x64.addArgument(Argument("", "", "r8"));
    x64.addArgument(Argument("", "", "r9"));
    mCallingConventions.push_back(x64);
#else
    CallingConvention x32(tr("Default (stdcall)"), 5);
    mCallingConventions.push_back(x32);

    CallingConvention x32ebp(tr("Default (stdcall, EBP stack)"), 5, "ebp");
    mCallingConventions.push_back(x32ebp);

    CallingConvention thiscall(tr("thiscall"), 4);
    thiscall.addArgument(Argument("this", "ecx", ""));
    mCallingConventions.push_back(thiscall);

    CallingConvention delphi(tr("Delphi (Borland fastcall)"), 2);
    delphi.addArgument(Argument("", "eax", ""));
    delphi.addArgument(Argument("", "edx", ""));
    delphi.addArgument(Argument("", "ecx", ""));
    mCallingConventions.push_back(delphi);
#endif //_WIN64

    for(auto & cur : mCallingConventions)
        ui->comboCallingConvention->addItem(cur.name);
}

void CPUArgumentWidget::setupTable()
{
    connect(mTable, SIGNAL(contextMenuSignal(QPoint)), this, SLOT(contextMenuSlot(QPoint)));
    mTable->verticalScrollBar()->setStyleSheet(ConfigVScrollBarStyle());
    mTable->enableMultiSelection(false);
    mTable->setShowHeader(false);
    mTable->addColumnAt(0, "", false);
    mTable->reloadData();
}

void CPUArgumentWidget::on_comboCallingConvention_currentIndexChanged(int index)
{
    mCurrentCallingConvention = index;
    const auto & cur = mCallingConventions[index];
    ui->spinArgCount->setValue(int(cur.arguments.size()) + cur.stackArgCount); //set the default argument count
    refreshData();
}

void CPUArgumentWidget::on_spinArgCount_valueChanged(int)
{
    mTable->setTableOffset(0); //reset the view to the first argument (fixes an ugly issue after refreshing)
    refreshData();
}

void CPUArgumentWidget::on_checkBoxLock_stateChanged(int)
{
    switch(ui->checkBoxLock->checkState())
    {
    case Qt::Checked:
        refreshData(); //first refresh then lock
        ui->checkBoxLock->setText(tr("Locked"));
        ui->spinArgCount->setEnabled(false);
        ui->comboCallingConvention->setEnabled(false);
        mAllowUpdate = false;
        break;
    case Qt::PartiallyChecked:
        refreshData(); //first refresh then lock
        ui->checkBoxLock->setText(tr("Calls"));
        ui->spinArgCount->setEnabled(false);
        ui->comboCallingConvention->setEnabled(false);
        mAllowUpdate = false;
        break;
    case Qt::Unchecked:
        ui->checkBoxLock->setText(tr("Unlocked"));
        ui->spinArgCount->setEnabled(true);
        ui->comboCallingConvention->setEnabled(true);
        mAllowUpdate = true;
        refreshData(); //first lock then refresh
        break;
    default:
        break;
    }
}
