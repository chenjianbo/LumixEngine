#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include <qsettings.h>
#include "assetbrowser.h"
#include "editor/world_editor.h"
#include "fileserverwidget.h"
#include "gameview.h"
#include "log_widget.h"
#include "property_view.h"
#include "sceneview.h"
#include "scripts/scriptcompilerwidget.h"
#include "materialmanager.h"
#include "profilerui.h"


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);
	m_ui->centralWidget->hide();
	setDockOptions(AllowNestedDocks | AnimatedDocks | AllowTabbedDocks);

	m_log = new LogWidget;
	m_property_view = new PropertyView;
	m_scene_view = new SceneView;
	m_game_view = new GameView;
	m_asset_browser = new AssetBrowser;
	m_script_compiler_ui = new ScriptCompilerWidget;
	m_file_server_ui = new FileServerWidget;
	m_material_manager_ui = new MaterialManager;
	m_profiler_ui = new ProfilerUI;

	QSettings settings("Lumix", "QtEditor");
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	
	addDockWidget(static_cast<Qt::DockWidgetArea>(1), m_game_view);
	addDockWidget(static_cast<Qt::DockWidgetArea>(8), m_log);
	addDockWidget(static_cast<Qt::DockWidgetArea>(8), m_file_server_ui);
	addDockWidget(static_cast<Qt::DockWidgetArea>(8), m_script_compiler_ui);
	addDockWidget(static_cast<Qt::DockWidgetArea>(1), m_property_view);
	addDockWidget(static_cast<Qt::DockWidgetArea>(2), m_scene_view);
	addDockWidget(static_cast<Qt::DockWidgetArea>(2), m_asset_browser);
	addDockWidget(static_cast<Qt::DockWidgetArea>(8), m_material_manager_ui);
	addDockWidget(static_cast<Qt::DockWidgetArea>(1), m_profiler_ui);

	m_property_view->setScriptCompiler(m_script_compiler_ui->getCompiler());

	restoreState(settings.value("mainWindowState").toByteArray());
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("Lumix", "QtEditor");
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
	delete m_log;
	delete m_ui;
	delete m_scene_view;
	delete m_property_view;
	delete m_game_view;
	delete m_asset_browser;
	delete m_script_compiler_ui;
	delete m_file_server_ui;
	delete m_material_manager_ui;
	delete m_profiler_ui;
}


void MainWindow::setWorldEditor(Lumix::WorldEditor& server)
{
	m_server = &server;
	m_file_server_ui->setWorldEditor(server);
	m_asset_browser->setWorldEditor(server);
	m_material_manager_ui->setWorldEditor(server);
	m_property_view->setWorldEditor(server);
}

GameView* MainWindow::getGameView() const
{
	return m_game_view;
}


SceneView* MainWindow::getSceneView() const
{
	return m_scene_view;
}


void MainWindow::on_actionLog_triggered()
{
	m_log->show();
}


void MainWindow::on_actionOpen_triggered()
{
	QByteArray path = QFileDialog::getOpenFileName(NULL, QString(), QString(), "universe (*.unv)").toLocal8Bit();
	if (!path.isEmpty())
	{
		m_server->loadUniverse(path.data());
	}
}

void MainWindow::on_actionSave_As_triggered()
{
	QByteArray path = QFileDialog::getSaveFileName().toLocal8Bit();
	if (!path.isEmpty())
	{
		m_server->saveUniverse(path.data());
	}
}

void MainWindow::on_actionCreate_triggered()
{
	m_server->addEntity();
}

void MainWindow::on_actionProperties_triggered()
{
	m_property_view->show();
}

void MainWindow::on_actionE_xit_triggered()
{
	close();
}

void MainWindow::on_actionGame_view_triggered()
{
	m_game_view->show();
}

void MainWindow::on_actionScript_compiler_triggered()
{
	m_script_compiler_ui->show();
}

void MainWindow::on_actionFile_server_triggered()
{
	m_file_server_ui->show();
}

void MainWindow::on_actionAsset_Browser_triggered()
{
	m_asset_browser->show();
}

void MainWindow::on_actionScene_View_triggered()
{
	m_scene_view->show();
}

void MainWindow::on_actionProfiler_triggered()
{
	m_profiler_ui->show();
}

void MainWindow::on_actionMaterial_manager_triggered()
{
    m_material_manager_ui->show();
}

void MainWindow::on_actionPolygon_Mode_changed()
{
	m_server->setWireframe(m_ui->actionPolygon_Mode->isChecked());
}

void MainWindow::on_actionGame_mode_triggered()
{
	m_server->toggleGameMode();
}

void MainWindow::on_actionLook_at_selected_entity_triggered()
{
	m_server->lookAtSelected();
}

void MainWindow::on_actionNew_triggered()
{
	m_server->newUniverse();
}

void MainWindow::on_actionSave_triggered()
{
	if (m_server->getUniversePath()[0] == '\0')
	{
		on_actionSave_As_triggered();
	}
	else
	{
		m_server->saveUniverse(m_server->getUniversePath());
	}
}
