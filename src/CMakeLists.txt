add_executable(blitz-backup)

target_sources(blitz-backup PRIVATE
    main.cpp
    backend.cpp
    resources.qrc
    snapshot.cpp
    snapshotmodel.cpp
)

target_link_libraries(blitz-backup
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::QuickControls2
    Qt${QT_MAJOR_VERSION}::Widgets
    KF${QT_MAJOR_VERSION}::CoreAddons
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::ConfigCore
    KF${QT_MAJOR_VERSION}::ConfigGui
)

kconfig_add_kcfg_files(blitz-backup GENERATE_MOC config.kcfgc)

install(TARGETS blitz-backup ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
