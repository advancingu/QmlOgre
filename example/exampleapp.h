#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include <QObject>

class ExampleApp : public QObject
{
    Q_OBJECT
public:
    explicit ExampleApp(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // EXAMPLEAPP_H
