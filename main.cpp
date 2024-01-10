#include <cstdlib>

#include <errno.h>
#include <unistd.h>

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>
#include <QIODevice>
#include <QString>

#include "mainwindow.hpp"

extern int errno;

#define error(...)                                                            \
	do {                                                                  \
		qCritical("error: " __VA_ARGS__);                             \
		std::exit(1);                                                 \
	} while (0)

namespace {

void
open_file(QApplication& app, QString filename)
{
	MainWindow* win = new MainWindow;
	QFile       f   = {filename};
	win->current    = f.fileName();
	if (!f.open(QIODevice::ReadOnly)) {
		error("couldn't open file '%s'",
				f.fileName().toStdString().c_str());
	}
	QByteArray contents = f.readAll();
	win->edit->setPlainText(contents);
	win->setWindowTitle(QString("%1 - Notepad").arg(f.fileName()));
	win->resize(800, 400);
	win->show();
}

} // namespace

int
main(int argc, char** argv)
{
	// Unless one of --wait or -w are specified as options, exit right
	// away.
	bool wait = false;
	for (int i = 0; i < argc; i += 1) {
		wait |= QString(argv[i]) == "--wait" ||
			QString(argv[i]) == "-w";
	}

	if (!wait) {
		pid_t pid = fork();
		if (pid < 0) {
			qFatal("error: failed to create new process: %s",
					strerror(errno));
		} else if (pid > 0) { // parent
			return 0;
		}
	}

	QApplication       app = {argc, argv};
	QCommandLineParser args;
	QCoreApplication::setOrganizationName("Ayman El Didi");
	QCoreApplication::setApplicationName("Notepad");
	QCoreApplication::setApplicationVersion("0.1.0");
	args.setApplicationDescription(QCoreApplication::applicationName());
	args.addHelpOption();
	args.addVersionOption();
	QCommandLineOption wait_opt = {{"w", "wait"},
			"Wait for the file to be closed before exiting"};
	args.addOption(wait_opt);
	args.addPositionalArgument("file...", "The files to open.");
	args.process(app);

	if (args.positionalArguments().length() > 1) {
		if (wait) {
			error("cannot specify --wait (-w) with "
			      "multiple files");
		}

		app.setQuitOnLastWindowClosed(false);
	}

	for (const auto& arg : args.positionalArguments()) {
		open_file(app, arg);
	}

	// Blank file
	return app.exec();
}
