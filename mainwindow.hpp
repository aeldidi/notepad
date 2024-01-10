#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <cstddef>

#include <QApplication>
#include <QKeySequence>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QVariant>
#include "qnamespace.h"

template <typename T> class Option : public QVariant {
public:
	Option()
	{
	}

	Option(T value) : t{value}
	{
	}

	void operator=(T other)
	{
		t = other;
	}

	void operator=(std::nullptr_t other)
	{
		t.clear();
	}

	T value() const
	{
		return t.value<T>();
	}

private:
	QVariant t;
};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow() : font{"monospace"}, edit{new QPlainTextEdit}
	{
		setCentralWidget(edit);
		font.setFixedPitch(true);
		font.setStyleHint(QFont::Monospace);
		font.setPointSize(QApplication::font().pointSize());
		edit->setFont(font);

		QShortcut* ctrl_plus = new QShortcut(
				QKeySequence(Qt::CTRL | Qt::Key_Equal), this);
		QObject::connect(ctrl_plus, &QShortcut::activated,
				[this]() { this->zoom_in(); });

		QShortcut* ctrl_minus = new QShortcut(
				QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
		QObject::connect(ctrl_minus, &QShortcut::activated,
				[this]() { this->zoom_out(); });
	}

	void zoom_in()
	{
		if (font.pointSize() >= 100) {
			return;
		}
		font.setPointSize(font.pointSize() + 1);
		edit->setFont(font);
	}

	void zoom_out()
	{
		if (font.pointSize() <= 1) {
			return;
		}
		font.setPointSize(font.pointSize() - 1);
		edit->setFont(font);
	}

	QFont           font;
	QPlainTextEdit* edit;
	Option<QString> current;
};

#endif // MAIN_WINDOW_H
