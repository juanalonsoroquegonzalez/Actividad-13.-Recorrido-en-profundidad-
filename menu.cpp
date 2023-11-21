#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QLineF>
#include <QGraphicsItem>
#include <QPen>
#include <QColor>
#include <QStack>
#include <QBrush>

#include "AdministradorNeuronas.h"
#include "menu.h"
#include "Neurona.h"
#include "./ui_menu.h"

Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_buttonAgregar_clicked()
{
    if(ui->lineID->text().isEmpty() || ui->lineVoltaje->text().isEmpty() || ui->lineX->text().isEmpty() || ui->lineY->text().isEmpty() || ui->lineRed->text().isEmpty() || ui->lineGreen->text().isEmpty() || ui->lineBlue->text().isEmpty()){
        ui->labelVerificar->setText("Datos incompletos");
    }
    else {
        ui->labelVerificar->setText("");

        QString id = ui->lineID->text();
        QString voltaje = ui->lineVoltaje->text();
        QString x = ui->lineX->text();
        QString y = ui->lineY->text();
        QString red = ui->lineRed->text();
        QString green = ui->lineGreen->text();
        QString blue = ui->lineBlue->text();

        Neurona neuronas(id.toInt(), voltaje.toInt(), x.toInt(), y.toInt(), red.toInt(), green.toInt(), blue.toInt());

        administrador.agregarInicio(neuronas);

        // Formatear la información de las neuronas en una cadena de texto
        QString infoNeurona = "---------------------------\nID: " + id + "\n"
                              + "Voltaje: " + voltaje + "\n"
                              + "Posición X: " + x + "\n"
                              + "Posición Y: " + y + "\n"
                              + "Red: " + red + "\n"
                              + "Green: " + green + "\n"
                              + "Blue: " + blue + "\n"
                              + "---------------------------\n";


        ///agregarNeurona();
        ui->textNeuronas->moveCursor(QTextCursor::Start);
        ui->textNeuronas->insertPlainText(infoNeurona);

        QMessageBox::warning(nullptr, "Valido!", "Neurona agregada");

        ui->lineID->setText("");
        ui->lineVoltaje->setText("");
        ui->lineX->setText("");
        ui->lineY->setText("");
        ui->lineRed->setText("");
        ui->lineGreen->setText("");
        ui->lineBlue->setText("");

        std::cout << "Neuronas: \n";
        administrador.mostrar();
    }
}


void Menu::on_buttonAgregarFinal_clicked()
{
    if(ui->lineID->text().isEmpty() || ui->lineVoltaje->text().isEmpty() || ui->lineX->text().isEmpty() || ui->lineY->text().isEmpty() || ui->lineRed->text().isEmpty() || ui->lineGreen->text().isEmpty() || ui->lineBlue->text().isEmpty()){
        ui->labelVerificar->setText("Datos incompletos");
    }
    else {
        ui->labelVerificar->setText("");

        QString id = ui->lineID->text();
        QString voltaje = ui->lineVoltaje->text();
        QString x = ui->lineX->text();
        QString y = ui->lineY->text();
        QString red = ui->lineRed->text();
        QString green = ui->lineGreen->text();
        QString blue = ui->lineBlue->text();

        Neurona neuronas(id.toInt(), voltaje.toInt(), x.toInt(), y.toInt(), red.toInt(), green.toInt(), blue.toInt());

        administrador.agregarInicio(neuronas);

        // Formatear la información de las neuronas en una cadena de texto
        QString infoNeurona = "---------------------------\nID: " + id + "\n"
                              + "Voltaje: " + voltaje + "\n"
                              + "Posición X: " + x + "\n"
                              + "Posición Y: " + y + "\n"
                              + "Red: " + red + "\n"
                              + "Green: " + green + "\n"
                              + "Blue: " + blue + "\n"
                              + "---------------------------\n";

        ui->textNeuronas->append(infoNeurona);

        QMessageBox::warning(nullptr, "Valido!", "Neurona agregada");

        ui->lineID->setText("");
        ui->lineVoltaje->setText("");
        ui->lineX->setText("");
        ui->lineY->setText("");
        ui->lineRed->setText("");
        ui->lineGreen->setText("");
        ui->lineBlue->setText("");

        std::cout << "Neuronas: \n";
        administrador.mostrar();
    }
}

void Menu::guardarTablaEnArchivo(const QString &nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&archivo);

        // Obtener el número de filas y columnas del QTableWidget
        int filas = ui->tableNeuronas->rowCount();
        int columnas = ui->tableNeuronas->columnCount();

        // Escribir las cabeceras de las columnas en el archivo
        QStringList cabeceras;
        for (int columna = 0; columna < columnas; ++columna) {
            cabeceras << ui->tableNeuronas->horizontalHeaderItem(columna)->text();
        }
        out << cabeceras.join("\t|") << '\n';

        // Escribir los datos de las celdas en el archivo
        for (int fila = 0; fila < filas; ++fila) {
            QStringList filaDatos;
            for (int columna = 0; columna < columnas; ++columna) {
                QTableWidgetItem *item = ui->tableNeuronas->item(fila, columna);
                if (item) {
                    filaDatos << item->text();
                } else {
                    filaDatos << ""; // Si no hay dato en la celda, agregar un espacio en blanco
                }
            }
            out << filaDatos.join("\t|") << '\n'; // Usar "\t|" como separador
        }

        archivo.close();
        QMessageBox::information(nullptr, "Guardado", "La tabla se ha guardado en el archivo correctamente.");
    } else {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo para escritura.");
    }
}

void Menu::cargarTablaDesdeArchivo(const QString &nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);

        // Limpiar la tabla existente
        ui->tableNeuronas->clearContents();
        ui->tableNeuronas->setRowCount(0);

        // Leer la primera línea para obtener las cabeceras
        QString cabeceraLinea = in.readLine();
        QStringList cabeceras = cabeceraLinea.split("\t|", Qt::SkipEmptyParts);

        // Establecer las cabeceras en la tabla
        ui->tableNeuronas->setColumnCount(cabeceras.size());
        ui->tableNeuronas->setHorizontalHeaderLabels(cabeceras);

        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList datos = linea.split("\t|", Qt::SkipEmptyParts);

            if (datos.size() == cabeceras.size()) {
                // Crear una instancia de Neurona con los datos
                int id = datos[0].toInt();
                float voltaje = datos[1].toFloat();
                int x = datos[2].toInt();
                int y = datos[3].toInt();
                int red = datos[4].toInt();
                int green = datos[5].toInt();
                int blue = datos[6].toInt();

                Neurona neurona(id, voltaje, x, y, red, green, blue);

                // Agregar la neurona a la clase AdministradorNeuronas
                administrador.agregarInicio(neurona);

                // Agregar la neurona a la tabla (opcional)
                int rowCount = ui->tableNeuronas->rowCount();
                ui->tableNeuronas->insertRow(rowCount);

                for (int i = 0; i < cabeceras.size(); ++i) {
                    QTableWidgetItem *item = new QTableWidgetItem(datos[i]);
                    ui->tableNeuronas->setItem(rowCount, i, item);
                }
            }
        }

        archivo.close();
        QMessageBox::information(nullptr, "Cargado", "El contenido se ha cargado desde el archivo correctamente.");
    } else {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo para lectura.");
    }
}

void Menu::on_buttonSave_clicked()
{
   QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo de texto"), "", tr("Archivos de Texto (*.txt);;Todos los archivos (*)"));

    if (fileName.isEmpty()) {
        // El usuario canceló la selección del archivo
        return;
    }

    // Abre el archivo en modo escritura
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo abrir el archivo para escritura."));
        return;
    }

    // Obtiene el contenido del QTextEdit
    QString contenido = ui->textNeuronas->toPlainText();

    // Escribe el contenido en el archivo
    QTextStream out(&file);
    out << contenido;

    ui->textNeuronas->clear();

    // Cierra el archivo
    file.close();

    QMessageBox::information(this, tr("Guardado"), tr("El contenido se ha guardado en el archivo correctamente."));
}


void Menu::on_buttonRead_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar archivo de texto"), "", tr("Archivos de Texto (*.txt);;Todos los archivos (*)"));

    if (fileName.isEmpty()) {
        // El usuario canceló la selección del archivo
        return;
    }

    // Abre el archivo en modo lectura
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo abrir el archivo para lectura."));
        return;
    }

    // Lee el contenido del archivo
    QTextStream in(&file);
    QString contenido = in.readAll();

    // Cierra el archivo
    file.close();

    // Coloca el contenido en el QTextEdit
    ui->textNeuronas->clear();
    ui->textNeuronas->setPlainText(contenido);

    QFile archivo(fileName);

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);

        // Variables temporales para almacenar los valores de las neuronas
        int id, voltaje, x, y, red, green, blue;
        bool enNeurona = false;

        while (!in.atEnd()) {
            QString linea = in.readLine().trimmed(); // Leer una línea y quitar espacios en blanco

            if (linea.startsWith("ID:")) {
                id = linea.mid(3).toInt(); // Extraer el ID desde la línea
            } else if (linea.startsWith("Voltaje:")) {
                voltaje = linea.mid(9).toInt(); // Extraer el voltaje desde la línea
            } else if (linea.startsWith("Posición X:")) {
                x = linea.mid(12).toInt(); // Extraer la posición X desde la línea
            } else if (linea.startsWith("Posición Y:")) {
                y = linea.mid(12).toInt(); // Extraer la posición Y desde la línea
            } else if (linea.startsWith("Red:")) {
                red = linea.mid(5).toInt(); // Extraer el valor de Red desde la línea
            } else if (linea.startsWith("Green:")) {
                green = linea.mid(7).toInt(); // Extraer el valor de Green desde la línea
            } else if (linea.startsWith("Blue:")) {
                blue = linea.mid(6).toInt(); // Extraer el valor de Blue desde la línea
            } else if (linea.startsWith("---------------------------")) {
                if (enNeurona) {
                    // Crear una instancia de Neurona con los valores y agregarla a la lista
                    Neurona neurona(id, voltaje, x, y, red, green, blue);
                    administrador.agregarInicio(neurona);
                }
                // Restablecer las variables para la próxima neurona
                id = voltaje = x = y = red = green = blue = 0;
                enNeurona = !enNeurona; // Cambiar el estado a una nueva neurona
            }
        }

    archivo.close();
    QMessageBox::information(nullptr, "Cargado", "El contenido se ha cargado desde el archivo correctamente.");
    } else {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo para lectura.");
    }
}


void Menu::on_buttonClean_clicked()
{
    ui->textNeuronas->clear();
}


void Menu::on_buttonSaveTable_clicked()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this, "Guardar tabla", "", "Archivos de Texto (*.txt);;Todos los archivos (*)");
    if (!nombreArchivo.isEmpty()) {
        guardarTablaEnArchivo(nombreArchivo);
    }
}


void Menu::on_buttonImportar_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar archivo de texto"), "", tr("Archivos de Texto (*.txt);;Todos los archivos (*)"));

    if (!fileName.isEmpty()) {
        cargarTablaDesdeArchivo(fileName);
    }
}

void Menu::buscarNeuronaPorID(const QString &id)
{
    bool neuronaEncontrada = false;

    for (const Neurona &neurona : administrador.listaNeuronas)
    {
        if (neurona.getID() == id.toInt())
        {
            // La neurona fue encontrada, muestra su información en el QTextEdit
            ui->textBuscar->clear(); // Limpia el QTextEdit antes de mostrar la información
            ui->textBuscar->append("Información de la Neurona con ID " + id + ":\n");
            ui->textBuscar->append("ID: " + QString::number(neurona.getID()));
            ui->textBuscar->append("Voltaje: " + QString::number(neurona.getVoltaje()));
            ui->textBuscar->append("Posición X: " + QString::number(neurona.getPosicion_x()));
            ui->textBuscar->append("Posición Y: " + QString::number(neurona.getPosicion_y()));
            ui->textBuscar->append("Red: " + QString::number(neurona.getRed()));
            ui->textBuscar->append("Green: " + QString::number(neurona.getGreen()));
            ui->textBuscar->append("Blue: " + QString::number(neurona.getBlue()));
            ui->textBuscar->append("-----------");
            neuronaEncontrada = true;
            break; // Sal del bucle ya que encontraste la neurona
        }
    }

    if (!neuronaEncontrada)
    {
        // Si no se encontró la neurona, muestra un mensaje en el QTextEdit
        ui->textBuscar->clear();
        ui->textBuscar->append("No se encontró ninguna neurona con el ID " + id);
    }
}


void Menu::on_buttonBuscar_clicked()
{
    QString id;
    id = ui->lineBuscar->text();
    buscarNeuronaPorID(id);
}


void Menu::on_buttonAdd_clicked()
{
    if (ui->lineID->text().isEmpty() || ui->lineVoltaje->text().isEmpty() || ui->lineX->text().isEmpty() || ui->lineY->text().isEmpty() || ui->lineRed->text().isEmpty() || ui->lineGreen->text().isEmpty() || ui->lineBlue->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Invalido!", "Datos Incompletos");
    }
    else {
        ui->labelVerificar->setText("");

        QString id = ui->lineID->text();
        QString voltaje = ui->lineVoltaje->text();
        QString x = ui->lineX->text();
        QString y = ui->lineY->text();
        QString red = ui->lineRed->text();
        QString green = ui->lineGreen->text();
        QString blue = ui->lineBlue->text();

        Neurona neuronas(id.toInt(), voltaje.toFloat(), x.toInt(), y.toInt(), red.toInt(), green.toInt(), blue.toInt());

        administrador.agregarInicio(neuronas);

        int rowCount = ui->tableNeuronas->rowCount();
        ui->tableNeuronas->insertRow(rowCount);
        // Cambiar el nombre de la cabecera de la columna "ID"
        QTableWidgetItem *cabeceraID = new QTableWidgetItem("ID");
        ui->tableNeuronas->setHorizontalHeaderItem(0, cabeceraID);

        // Cambiar el nombre de la cabecera de la columna "VOLTAJE"
        QTableWidgetItem *cabeceraVoltaje = new QTableWidgetItem("VOLTAJE");
        ui->tableNeuronas->setHorizontalHeaderItem(1, cabeceraVoltaje);

        // Cambiar el nombre de la cabecera de la columna "X"
        QTableWidgetItem *cabeceraX = new QTableWidgetItem("X");
        ui->tableNeuronas->setHorizontalHeaderItem(2, cabeceraX);

        // Cambiar el nombre de la cabecera de la columna "Y"
        QTableWidgetItem *cabeceraY = new QTableWidgetItem("Y");
        ui->tableNeuronas->setHorizontalHeaderItem(3, cabeceraY);

        // Cambiar el nombre de la cabecera de la columna "RED"
        QTableWidgetItem *cabeceraRed = new QTableWidgetItem("RED");
        ui->tableNeuronas->setHorizontalHeaderItem(4, cabeceraRed);

        // Cambiar el nombre de la cabecera de la columna "GREEN"
        QTableWidgetItem *cabeceraGreen = new QTableWidgetItem("GREEN");
        ui->tableNeuronas->setHorizontalHeaderItem(5, cabeceraGreen);

        // Cambiar el nombre de la cabecera de la columna "BLUE"
        QTableWidgetItem *cabeceraBlue = new QTableWidgetItem("BLUE");
        ui->tableNeuronas->setHorizontalHeaderItem(6, cabeceraBlue);

        QTableWidgetItem *itemID = new QTableWidgetItem(id);
        QTableWidgetItem *itemVoltaje = new QTableWidgetItem(voltaje);
        QTableWidgetItem *itemX = new QTableWidgetItem(x);
        QTableWidgetItem *itemY = new QTableWidgetItem(y);
        QTableWidgetItem *itemRed = new QTableWidgetItem(red);
        QTableWidgetItem *itemGreen = new QTableWidgetItem(green);
        QTableWidgetItem *itemBlue = new QTableWidgetItem(blue);

        ui->tableNeuronas->setItem(rowCount, 0, itemID);
        ui->tableNeuronas->setItem(rowCount, 1, itemVoltaje);
        ui->tableNeuronas->setItem(rowCount, 2, itemX);
        ui->tableNeuronas->setItem(rowCount, 3, itemY);
        ui->tableNeuronas->setItem(rowCount, 4, itemRed);
        ui->tableNeuronas->setItem(rowCount, 5, itemGreen);
        ui->tableNeuronas->setItem(rowCount, 6, itemBlue);

        QMessageBox::warning(nullptr, "Valido!", "Neurona agregada");

        ui->lineID->setText("");
        ui->lineVoltaje->setText("");
        ui->lineX->setText("");
        ui->lineY->setText("");
        ui->lineRed->setText("");
        ui->lineGreen->setText("");
        ui->lineBlue->setText("");

        std::cout << "Neuronas: \n";
        administrador.mostrar();
    }
}


void Menu::on_pushButton_clicked()
{
    QString voltaje = ui->lineVoltaje->text();
    QString x = ui->lineX->text();
    QString y = ui->lineY->text();
    QString red = ui->lineRed->text();
    QString green = ui->lineGreen->text();
    QString blue = ui->lineBlue->text();

    QPen pen;
    QColor color(red.toInt(), green.toInt(), blue.toInt());
    pen.setColor(color);
    QBrush brush(color, Qt::Dense4Pattern);
    scene.addEllipse(x.toInt(), y.toInt(), voltaje.toInt(), voltaje.toInt(), pen, brush);

    // Agregar el ID de la neurona
    QString idNeurona = "ID: " + ui->lineID->text();
    scene.addText(idNeurona, QFont("Arial", 9, QFont::Bold))->setPos(x.toInt(), y.toInt());

    Neurona nuevaNeurona(ui->lineID->text().toInt(), voltaje.toDouble(), x.toInt(), y.toInt(), red.toInt(), green.toInt(), blue.toInt());

    // Agregar la neurona a la lista de neuronas
    try {
        administrador.agregarInicio(nuevaNeurona);
    } catch (const std::runtime_error& e) {
        qDebug() << "Error al agregar neurona: " << e.what();
    }

    ui->graphicsViewNeuronas->setScene(&scene);

}


void Menu::on_pushButton_2_clicked()
{
    ui->graphicsViewNeuronas->scene()->clear();
}


void Menu::on_pushButtonOrdenarText_clicked()
{
    administrador.ordenarPorID();
    ui->textNeuronas->clear();

    // Iterar a través de las neuronas ordenadas y mostrarlas en textNeuronas
    for (const Neurona& neurona : administrador.listaNeuronas) {
        QString infoNeurona = "---------------------------\nID: " + QString::number(neurona.getID()) + "\n"
                              + "Voltaje: " + QString::number(neurona.getVoltaje()) + "\n"
                              + "Posición X: " + QString::number(neurona.getPosicion_x()) + "\n"
                              + "Posición Y: " + QString::number(neurona.getPosicion_y()) + "\n"
                              + "Red: " + QString::number(neurona.getRed()) + "\n"
                              + "Green: " + QString::number(neurona.getGreen()) + "\n"
                              + "Blue: " + QString::number(neurona.getBlue()) + "\n"
                              + "---------------------------\n";

        ui->textNeuronas->append(infoNeurona);
    }
}


void Menu::on_pushButtonOrdenarText2_clicked()
{
    administrador.ordenarPorVoltaje();
    ui->textNeuronas->clear();

    // Iterar a través de las neuronas ordenadas y mostrarlas en textNeuronas
    for (const Neurona& neurona : administrador.listaNeuronas) {
        QString infoNeurona = "---------------------------\nID: " + QString::number(neurona.getID()) + "\n"
                              + "Voltaje: " + QString::number(neurona.getVoltaje()) + "\n"
                              + "Posición X: " + QString::number(neurona.getPosicion_x()) + "\n"
                              + "Posición Y: " + QString::number(neurona.getPosicion_y()) + "\n"
                              + "Red: " + QString::number(neurona.getRed()) + "\n"
                              + "Green: " + QString::number(neurona.getGreen()) + "\n"
                              + "Blue: " + QString::number(neurona.getBlue()) + "\n"
                              + "---------------------------\n";

        ui->textNeuronas->append(infoNeurona);
    }
}

void Menu::cargarNeuronasEnTabla(const QList<Neurona> &neuronas) {
    ui->tableNeuronas->setRowCount(neuronas.size());

    for (int i = 0; i < neuronas.size(); ++i) {
        const Neurona& neurona = neuronas[i];

        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(neurona.getID()));
        QTableWidgetItem *itemVoltaje = new QTableWidgetItem(QString::number(neurona.getVoltaje()));
        QTableWidgetItem *itemX = new QTableWidgetItem(QString::number(neurona.getPosicion_x()));
        QTableWidgetItem *itemY = new QTableWidgetItem(QString::number(neurona.getPosicion_y()));
        QTableWidgetItem *itemRed = new QTableWidgetItem(QString::number(neurona.getRed()));
        QTableWidgetItem *itemGreen = new QTableWidgetItem(QString::number(neurona.getGreen()));
        QTableWidgetItem *itemBlue = new QTableWidgetItem(QString::number(neurona.getBlue()));

        ui->tableNeuronas->setItem(i, 0, itemID);
        ui->tableNeuronas->setItem(i, 1, itemVoltaje);
        ui->tableNeuronas->setItem(i, 2, itemX);
        ui->tableNeuronas->setItem(i, 3, itemY);
        ui->tableNeuronas->setItem(i, 4, itemRed);
        ui->tableNeuronas->setItem(i, 5, itemGreen);
        ui->tableNeuronas->setItem(i, 6, itemBlue);
    }
}

void Menu::on_pushButtonOrdenarTable_clicked()
{
    administrador.ordenarPorID(); // Llama a la función de ordenamiento por ID

    // Convierte la lista de neuronas de std::list a QList
    QList<Neurona> listaNeuronas;
    for (const Neurona &neurona : administrador.listaNeuronas) {
        listaNeuronas.append(neurona);
    }

    // Carga las neuronas ordenadas en la tabla
    cargarNeuronasEnTabla(listaNeuronas);
}


void Menu::on_pushButtonOrdenarTable2_clicked()
{
    administrador.ordenarPorVoltaje(); // Llama a la función de ordenamiento por ID

    // Convierte la lista de neuronas de std::list a QList
    QList<Neurona> listaNeuronas;
    for (const Neurona &neurona : administrador.listaNeuronas) {
        listaNeuronas.append(neurona);
    }

    // Carga las neuronas ordenadas en la tabla
    cargarNeuronasEnTabla(listaNeuronas);
}


void Menu::on_pushLimpiar_clicked()
{
    administrador.listaNeuronas.clear();
}


void Menu::on_buttonFuerza_clicked()
{
    QList<QGraphicsItem *> items = scene.items();
    QPen lineaPen;
    lineaPen.setColor(Qt::black); // Color de la línea

    QFont font;
    font.setBold(true);

    QList<QGraphicsEllipseItem *> circulos;
    for (QGraphicsItem *item : items) {
        if (dynamic_cast<QGraphicsEllipseItem*>(item)) {
            circulos.append(static_cast<QGraphicsEllipseItem*>(item));
        }
    }

    QVector<QGraphicsLineItem *> lineas; // Almacenar las líneas para agregar las flechas después

    while (!circulos.isEmpty()) {
        QGraphicsEllipseItem *circulo1 = circulos.takeFirst();
        QPointF centro1 = circulo1->boundingRect().center();
        qreal distanciaMinima = std::numeric_limits<qreal>::max(); // Valor máximo de punto flotante
        QGraphicsEllipseItem *circuloMasCercano = nullptr;

        for (QGraphicsEllipseItem *circulo2 : circulos) {
            QPointF centro2 = circulo2->boundingRect().center();
            qreal distancia = QLineF(centro1, centro2).length();

            if (distancia < distanciaMinima) {
                distanciaMinima = distancia;
                circuloMasCercano = circulo2;
            }
        }

        if (circuloMasCercano) {
            QPointF centroCercano = circuloMasCercano->boundingRect().center();
            QLineF linea(centro1, centroCercano);

            // Almacena la línea en el vector para agregar flechas después
            QGraphicsLineItem *lineaItem = scene.addLine(linea, lineaPen);
            lineas.append(lineaItem);
            double distancia = std::sqrt(std::pow(centroCercano.rx() - centro1.rx(), 2) + std::pow(centroCercano.ry() - centro1.ry(), 2));
            // Calcula el punto medio de la línea para mostrar el ID
            QPointF puntoMedio = linea.pointAt(0.5);

            QString id = QString::number(int(distancia));
            qDebug() << "ID neurona actual:" << int(centro1.rx()) << int(centro1.ry());
            qDebug() << "ID neurona mas cercana:" << int(centroCercano.rx()) << int(centroCercano.ry());


            QGraphicsSimpleTextItem *idLabel = scene.addSimpleText(id);
            idLabel->setFont(font);
            idLabel->setPos(puntoMedio);
        }

    }

    // Agrega flechas a las líneas
    QPen flechaPen;
    flechaPen.setColor(Qt::black);
    flechaPen.setWidth(2);

    for (QGraphicsLineItem *lineaItem : lineas) {
        QLineF linea = lineaItem->line();
        QPolygonF flecha;
        flecha << QPointF(0, 0) << QPointF(5, 15) << QPointF(-5, 15);
        QTransform transform;
        transform.translate(linea.p2().x(), linea.p2().y());
        transform.rotate(-linea.angle());
        flecha = transform.map(flecha);
        scene.addPolygon(flecha, flechaPen, QBrush(Qt::black));
    }
}

void Menu::on_pushRecor_clicked()
{
    QList<QGraphicsItem *> items = scene.items();
    QPen lineaPen;
    lineaPen.setColor(Qt::black);
    const qreal DISTANCIA_MAXIMA_ENTRE_CIRCULOS = 1000.0;

    QFont font;
    font.setBold(true);
    QList<QString> visited;

    QList<QGraphicsEllipseItem *> circulos;
    for (QGraphicsItem *item : items) {
        if (dynamic_cast<QGraphicsEllipseItem*>(item)) {
            circulos.append(static_cast<QGraphicsEllipseItem*>(item));
        }
    }

    QVector<QGraphicsLineItem *> lineas;

    QSet<QGraphicsEllipseItem *> visitados;
    QStack<QGraphicsEllipseItem *> pila;
    while (!circulos.isEmpty()) {
        QGraphicsEllipseItem *circuloInicial = circulos.takeFirst();

        if (!visitados.contains(circuloInicial)) {
            pila.push(circuloInicial);

            while (!pila.isEmpty()) {
                QGraphicsEllipseItem *circuloActual = pila.pop();

                if (!visitados.contains(circuloActual)) {
                    visitados.insert(circuloActual);

                    QPointF centroActual = circuloActual->boundingRect().center();

                    Neurona encon = administrador.obtenerNeuronaDesdePosicion(centroActual.x() - 61.5, centroActual.y() - 61.5);
                    visited.append(QString::number(encon.getID()));



                        for (QGraphicsEllipseItem *circuloVecino : circulos) {
                        QPointF centroVecino = circuloVecino->boundingRect().center();
                        QLineF linea(centroActual, centroVecino);

                        if (linea.length() < DISTANCIA_MAXIMA_ENTRE_CIRCULOS) {
                            QGraphicsLineItem *lineaItem = scene.addLine(linea, lineaPen);
                            lineas.append(lineaItem);

                            pila.push(circuloVecino);
                        }
                    }
                    qDebug() << "Recorrido:" << visited;
                    ui->textRecorrido->insertPlainText("Recorrido: " + visited.join(", "));
                }
            }
        }
    }
}

