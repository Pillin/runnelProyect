#include "toolbarsconfigmesh.h"
#include "ui_toolbarsconfigmesh.h"
#include <iostream>
#include <QMessageBox>
#include "patternsdata.h"
#include "drainageAlgorithms/gradientdrainagecallaghanmark.h"
#include "drainageAlgorithms/peuckerdrainagenetwork.h"
#include "drainageAlgorithms/nonedrainagealgorithm.h"
#include "drainageAlgorithms/diedralangledrainage.h"
#include "buildNetwork/nonebuildnetwork.h"
#include "buildNetwork/buildtreecallaghan.h"
#include "buildNetwork/buildtreepeucker.h"
#include "patternsAlgorithms/nonepatronalgorithm.h"
#include "patternsAlgorithms/zhangguilbertalgorithm.h"
#include "waterPathAlgorithms/nonepathwateralgorithm.h"
#include "waterPathAlgorithms/pathwatercallaghanalgorithm.h"
#include "waterPathAlgorithms/pathwatergradientalgorithm.h"



ToolbarsConfigMesh::ToolbarsConfigMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolbarsConfigMesh)
{
    ui->setupUi(this);
    QObject::connect(ui->drainage_patron_button, SIGNAL(clicked()), this, SLOT(getPatron()));
    QObject::connect(ui->river_button, SIGNAL(clicked()), this, SLOT(getDrainage()));
    QObject::connect(ui->path_water_button, SIGNAL(clicked()), this, SLOT(getWater()));
    QObject::connect(ui->network_button, SIGNAL(clicked()), this, SLOT(getNetwork()));
    QObject::connect(ui->exaggeration_terrain_value, SIGNAL(valueChanged(int)), this, SIGNAL(changeElevation(int)));
    QObject::connect(ui->landform_value, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changeLandForm(int)));
    QObject::connect(ui->gradient_vector, SIGNAL(clicked(bool)), this, SIGNAL(showGradientVector(bool)));
    QObject::connect(ui->normal_vector, SIGNAL(clicked(bool)), this, SIGNAL(showNormalVector(bool)));
    QObject::connect(ui->coordinate_axis, SIGNAL(clicked(bool)), this, SIGNAL(showCoordinateAxis(bool)));
    QObject::connect(ui->patterns_information, SIGNAL(clicked()), this, SLOT(showPatternsInformation()));
    this->drainageIncludeAlgorithms();
    this->pathWaterIncludeAlgorithms();
    this->networkIncludeAlgorithms();
    this->patronIncludeAlgorithms();
}

ToolbarsConfigMesh::~ToolbarsConfigMesh()
{
    // Clear conf's layouts to prevent double delete.
    while (ui->river_conf->layout()->takeAt(0) != 0);
    while (ui->path_conf->layout()->takeAt(0) != 0);
    while (ui->network_conf->layout()->takeAt(0) != 0);
    while (ui->patron_conf->layout()->takeAt(0) != 0);
    for(auto alg: drainage_algorithms)
        delete alg;
    for(auto alg: path_water_algorithms)
        delete alg;
    for(auto alg: patron_algorithms)
        delete alg;
    for(auto alg: network_algorithms)
        delete alg;
    delete ui;
}

void ToolbarsConfigMesh::drainageIncludeAlgorithms(){
    //Add new gradient algorithms
    drainage_algorithms.push_back(new NoneDrainageAlgorithm());
    drainage_algorithms.push_back(new PeuckerDrainageNetwork());
    drainage_algorithms.push_back(new DiedralAngleDrainage());
    drainage_algorithms.push_back(new GradientDrainageCallaghanMark());
    for( DrainageAlgorithms* item : drainage_algorithms){
        ui->river_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getDrainage(){
    int number = ui->river_value->currentIndex();
    DrainageAlgorithms* item = drainage_algorithms[number];
    if(item->getConf()){
        ui->river_conf->layout()->takeAt(0);
        ui->river_conf->layout()->addWidget(item->getConf());
    }
    emit selectDrainage(item);
}

void ToolbarsConfigMesh::networkIncludeAlgorithms(){
    network_algorithms.push_back(new NoneBuildNetwork());
    network_algorithms.push_back(new buildTreeCallaghan());
    network_algorithms.push_back(new BuildTreePeucker());
    for( BuildNetwork* item : network_algorithms){
        ui->network_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getNetwork(){
    int number = ui->network_value->currentIndex();
    BuildNetwork* item = network_algorithms[number];
    if(item->getConf()){
        ui->network_conf->layout()->takeAt(0);
        ui->network_conf->layout()->addWidget(item->getConf());
    }
    emit selectNetwork(item);
}

void ToolbarsConfigMesh::patronIncludeAlgorithms(){
    patron_algorithms.push_back(new NonePatronAlgorithm());
    patron_algorithms.push_back(new ZhangGuilbertAlgorithm());
    for( AlgorithmPatron* item : patron_algorithms){
        ui->drainage_patron_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getPatron(){
    int number = ui->drainage_patron_value->currentIndex();
    AlgorithmPatron* item = patron_algorithms[number];
    if(item->getConf()){
        ui->patron_conf->layout()->takeAt(0);
        ui->patron_conf->layout()->addWidget(item->getConf());
    }
    emit selectPatron(item);
}

void ToolbarsConfigMesh::pathWaterIncludeAlgorithms(){
    path_water_algorithms.push_back(new NonePathWaterAlgorithm());
    path_water_algorithms.push_back(new PathWaterCallaghanAlgorithm());
    path_water_algorithms.push_back(new PathWaterGradientAlgorithm());
    for( PathWaterAlgorithm* item : path_water_algorithms){
        ui->path_water_value->addItem(item->getName());
    }
}

void ToolbarsConfigMesh::getWater(){
    int number = ui->path_water_value->currentIndex();
    PathWaterAlgorithm* item = path_water_algorithms[number];
    if(item->getConf()){
        ui->path_conf->layout()->takeAt(0);
        ui->path_conf->layout()->addWidget(item->getConf());
    }
    emit selectWater(item);
}

void ToolbarsConfigMesh::glewIsReady(){
    for( DrainageAlgorithms* item : drainage_algorithms){
        item->glewReady();
    }
    for( BuildNetwork* item : network_algorithms){
        item->glewReady();
    }
    for( PathWaterAlgorithm* item : path_water_algorithms){
        item->glewReady();
    }
    for( AlgorithmPatron* item : patron_algorithms){
        item->glewReady();
    }
}

void ToolbarsConfigMesh::showPatternsInformation(){

    QMessageBox s;
    QString title = "Patterns Information";
    QString information;
    information+="<table>";
    for( int i = 0; i < PatternsData::names_patterns.size(); ++i){
        information+= "<tr>";

        information+= "<td valign=\"middle\" >";
        information+= "<img src=\"" + PatternsData::images[i] +"\" >";
        information+= "</td>";

        information+= "<td valign=\"middle\" >";
        information+= "<b>" + PatternsData::names_patterns[i] + "</b>";
        information+= "<br>";
        information+= PatternsData::descriptions[i];
        information+= "<br>";
        information+= "</td>";

        information+= "</tr>";
    }
    information+="</table>";
    s.about(this, title, information);
}
