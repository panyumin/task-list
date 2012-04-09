#include "htmleditor.h"
#include <QtGui>

htmleditor::htmleditor(QWidget *parent) :
    QWidget(parent)
{

    textEdit = new QTextEdit;
    editToolBar = new QToolBar("edit");
    styleToolBar = new QToolBar("style");
    fontToolBar = new QToolBar("font");
    createActions();
    createToolBars();
    setup();

}

void htmleditor::createActions()
{
    //edit actions
    cutAct = new QAction(QIcon(":/style/all/images/cut.png"),tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));
    cutAct->setEnabled(false);
    connect(this->textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));

    copyAct = new QAction(QIcon(":/style/all/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
    copyAct->setEnabled(false);
    connect(this->textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

    pasteAct = new QAction(QIcon(":/style/all/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    undoAct = new QAction(QIcon(":/style/all/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    connect(undoAct, SIGNAL(triggered()), textEdit, SLOT(undo()));


    redoAct = new QAction(QIcon(":/style/all/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcut(tr("Ctrl+Y"));
    connect(redoAct, SIGNAL(triggered()), textEdit, SLOT(redo()));

    boldAct = new QAction(QIcon(":/style/all/images/bold.png"), tr("&Bold"), this);
    boldAct->setShortcut(tr("Ctrl+B"));
    QFont bold;
    bold.setBold(true);
    boldAct->setFont(bold);
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));
    boldAct->setCheckable(true);

    italicAct = new QAction(QIcon(":/style/all/images/italic.png"), tr("&Italic"), this);
    italicAct->setShortcut(tr("Ctrl+I"));
    QFont italic;
    italic.setItalic(true);
    italicAct->setFont(italic);
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));
    italicAct->setCheckable(true);


    underlineAct = new QAction(QIcon(":/style/all/images/underline.png"), tr("&Underline"), this);
    underlineAct->setShortcut(tr("Ctrl+U"));
    QFont underline;
    underline.setItalic(true);
    underlineAct->setFont(underline);
    connect(underlineAct, SIGNAL(triggered()), this, SLOT(underline()));
    underlineAct->setCheckable(true);

    strikethroughAct= new QAction(QIcon(":/style/all/images/strikethrough.png"), tr("&Strike Through"), this);
    //strikethroughAct->setShortcut(tr("Ctrl+B"));
    QFont strikethrough;
    strikethrough.setStrikeOut(true);
    strikethroughAct->setFont(strikethrough);
    connect(strikethroughAct, SIGNAL(triggered()), this, SLOT(strikethrough()));
    strikethroughAct->setCheckable(true);

    //style actions
    leftAct = new QAction(QIcon(":/style/all/images/left.png"), tr("&Align left"), this);
    leftAct->setShortcut(tr("Ctrl+L"));
    connect(leftAct, SIGNAL(triggered()), this, SLOT(ali_left()));
    //leftAct->setCheckable(true);

    centerAct = new QAction(QIcon(":/style/all/images/center.png"), tr("&Align center"), this);
    centerAct->setShortcut(tr("Ctrl+E"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(ali_center()));
    //centerAct->setCheckable(true);

    rightAct = new QAction(QIcon(":/style/all/images/right.png"), tr("&Align right"), this);
    rightAct->setShortcut(tr("Ctrl+R"));
    connect(rightAct, SIGNAL(triggered()), this, SLOT(ali_right()));
    //rightAct->setCheckable(true);

    bulletAct = new QAction(QIcon(":/style/all/images/bullet.png"), tr("&Bulleted list"), this);
    connect(bulletAct, SIGNAL(triggered()), this, SLOT(bullet()));

    numberAct = new QAction(QIcon(":/style/all/images/number.png"), tr("&Numbered list"), this);
    connect(numberAct, SIGNAL(triggered()), this, SLOT(number()));

    indent_leftAct = new QAction(QIcon(":/style/all/images/indent_left.png"), tr("&Indent left"), this);
    connect(indent_leftAct, SIGNAL(triggered()), this, SLOT(indent_left()));

    indent_rightAct = new QAction(QIcon(":/style/all/images/indent_right.png"), tr("&Indent right"), this);
    connect(indent_rightAct, SIGNAL(triggered()), this, SLOT(indent_right()));


    QPixmap color_pix(16,16);
    color_pix.fill(Qt::black);
    colorAct = new QAction(color_pix,tr("&Text color"),this);
    connect(colorAct,SIGNAL(triggered()),this,SLOT(change_color()));


    font_size_comb = new QComboBox;
    font_size_comb->setObjectName("Font size");
    font_size_comb->setEditable(false);


    //we will add size from 8 to 24 to the font size combo box
    //and the step will be 1
    //so we can have font size of 8, 9, 10, ... till 24
    for(int i = 8 ; i!=26; i++ )
    {

        font_size_comb->addItem(QString::number(i));

    }

    connect(font_size_comb, SIGNAL(activated(QString)),this, SLOT(change_size(QString)));
    font_size_comb->setCurrentIndex(2);


    font_comb = new QFontComboBox;
    connect(font_comb, SIGNAL(activated(QString)),this, SLOT(change_font(QString)));
    font_comb->setEditable(false);


    connect(this->textEdit, SIGNAL(currentCharFormatChanged (QTextCharFormat)),this, SLOT(format_changed(QTextCharFormat)));

}

void htmleditor::createToolBars()
{
    //add actions to edit bar
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    editToolBar->addSeparator();

    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);

    editToolBar->addSeparator();

    editToolBar->addAction(boldAct);
    editToolBar->addAction(italicAct);
    editToolBar->addAction(underlineAct);
    editToolBar->addAction(strikethroughAct);

    //add actions to style bar
    styleToolBar->addAction(leftAct);
    styleToolBar->addAction(centerAct);
    styleToolBar->addAction(rightAct);

    styleToolBar->addSeparator();

    styleToolBar->addAction(bulletAct);
    styleToolBar->addAction(numberAct);
    styleToolBar->addAction(indent_leftAct);
    styleToolBar->addAction(indent_rightAct);

    styleToolBar->addSeparator();

    fontToolBar->addAction(colorAct);
    fontToolBar->addWidget(font_size_comb);
    fontToolBar->addWidget(font_comb);

}

void htmleditor::setup()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(editToolBar);
    layout->addWidget(styleToolBar);
    layout->addWidget(fontToolBar);
    layout->addWidget(textEdit);

    this->setLayout(layout);


}

void htmleditor::bold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);

    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);
}


void htmleditor::italic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());

    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);
}

void htmleditor::underline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());

    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);
}

void htmleditor::strikethrough()
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(strikethroughAct->isChecked());

    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);
}

void htmleditor::ali_left()
{
    this->textEdit->setAlignment(Qt::AlignLeft );
}

void htmleditor::ali_center()
{
    this->textEdit->setAlignment(Qt::AlignCenter );
}

void htmleditor::ali_right()
{
    this->textEdit->setAlignment(Qt::AlignRight);
}

void htmleditor::bullet()
{
    QTextCursor tmp_cursor = this->textEdit->textCursor();

    tmp_cursor.beginEditBlock();

    QTextBlockFormat block_format = tmp_cursor.blockFormat();

    QTextListFormat list_format;

    if (tmp_cursor.currentList())
    {
        list_format = tmp_cursor.currentList()->format();
    }
    else
    {
        list_format.setIndent(block_format.indent() + 1);
        block_format.setIndent(0);
        tmp_cursor.setBlockFormat(block_format);
    }

    list_format.setStyle(QTextListFormat::ListDisc);

    tmp_cursor.createList(list_format);

    tmp_cursor.endEditBlock();

}

void htmleditor::number()
{
    QTextCursor tmp_cursor = this->textEdit->textCursor();

    tmp_cursor.beginEditBlock();

    QTextBlockFormat block_format = tmp_cursor.blockFormat();

    QTextListFormat list_format;

    if (tmp_cursor.currentList())
    {
        list_format = tmp_cursor.currentList()->format();
    }
    else
    {
        list_format.setIndent(block_format.indent() + 1);
        block_format.setIndent(0);
        tmp_cursor.setBlockFormat(block_format);
    }

    list_format.setStyle(QTextListFormat::QTextListFormat::ListDecimal);

    tmp_cursor.createList(list_format);

    tmp_cursor.endEditBlock();

}

void htmleditor::indent_left()
{
    QTextCursor tmp_cursor = this->textEdit->textCursor();

    tmp_cursor.beginEditBlock();

    QTextBlockFormat block_format = tmp_cursor.blockFormat();

    if(block_format.indent()>0)
    {
        block_format.setIndent(block_format.indent()-1);

        tmp_cursor.setBlockFormat(block_format);
    }


    tmp_cursor.endEditBlock();

}


void htmleditor::indent_right()
{
    QTextCursor tmp_cursor = this->textEdit->textCursor();

    tmp_cursor.beginEditBlock();

    QTextBlockFormat block_format = tmp_cursor.blockFormat();

    block_format.setIndent(block_format.indent()+1);


    tmp_cursor.setBlockFormat(block_format);

    tmp_cursor.endEditBlock();


}

void htmleditor::change_color()
{
    QColor tmp_col = QColorDialog::getColor(this->textEdit->textColor(), this);

    if (!tmp_col.isValid())
    {
        return;
    }

    QTextCharFormat fmt;
    fmt.setForeground(tmp_col);

    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);

    QPixmap change_pix(16,16);
    change_pix.fill(tmp_col);

    this->colorAct->setIcon(change_pix);

}


void htmleditor::change_size(const QString &str)
{
    qreal font_size = str.toFloat();

    if (font_size > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(font_size);

        QTextCursor cursor = this->textEdit->textCursor();

        if (!cursor.hasSelection())
        {
            cursor.select(QTextCursor::WordUnderCursor);
        }

        cursor.mergeCharFormat(fmt);
        this->textEdit->mergeCurrentCharFormat(fmt);

    }

}


void htmleditor::change_font(const QString &str)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(str);

    QTextCursor cursor = this->textEdit->textCursor();

    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    cursor.mergeCharFormat(fmt);
    this->textEdit->mergeCurrentCharFormat(fmt);


}

void htmleditor::format_changed(const QTextCharFormat &format)
{

    //the following will make the tool bar to change status according to the location of the cursor
    font_comb->setCurrentIndex( font_comb->findText( QFontInfo(format.font()).family() ) );

    font_size_comb->setCurrentIndex( font_size_comb->findText( QString::number( format.font().pointSize() ) ) );

    if(format.font().bold() == true)
    {
        boldAct->setChecked(true);
    }
    else
    {
        boldAct->setChecked(false);
    }

    if(format.font().italic() == true)
    {
        italicAct->setChecked(true);
    }
    else
    {
        italicAct->setChecked(false);
    }


    if(format.font().underline() == true)
    {
        underlineAct->setChecked(true);
    }
    else
    {
        underlineAct->setChecked(false);
    }

    if(format.font().strikeOut() == true)
    {
        strikethroughAct->setChecked(true);
    }
    else
    {
        strikethroughAct->setChecked(false);
    }



    QPixmap change_pix(16,16);
    change_pix.fill(format.foreground().color());
    this->colorAct->setIcon(change_pix);


}

