#ifndef UNICORN_DIALOG_H_
#define UNICORN_DIALOG_H_

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "../UnicornMainWindow.h"
#include "../UnicornSettings.h"

namespace unicorn {
class Dialog : public QDialog
{
Q_OBJECT
public:
    Dialog( QWidget* parent = 0, Qt::WindowFlags f = 0 )
    :QDialog( parent, f ), m_firstShow( true )
    {
        connect( qApp->desktop(), SIGNAL( resized(int)), SLOT( cleverlyPosition()));
    }

    virtual void setVisible( bool visible )
    {
        if( !visible || !m_firstShow ) return QDialog::setVisible( visible );
        m_firstShow = false;

        cleverlyPosition();
        return QDialog::setVisible( visible );
    }

protected:
    void saveState( const QObject* object, const QString& key, const QVariant& value ) const
    {
        AppSettings s;
        s.beginGroup( QString( metaObject()->className()));
        s.beginGroup( QString( object->metaObject()->className()));
        s.setValue( key, value );
    }

    QVariant restoreState( const QObject* object, const QString& key, const QVariant& defaultValue = QVariant() ) const
    {
        AppSettings s;
        s.beginGroup( QString( metaObject()->className()));
        s.beginGroup( QString( object->metaObject()->className()));
        return s.value( key, defaultValue );
    }

    virtual void moveEvent( QMoveEvent* event )
    {
        using unicorn::MainWindow;
        QWidget* mw = findMainWindow();
        if( !mw ) return QDialog::moveEvent( event );

        AppSettings s;
        s.beginGroup( QString( metaObject()->className() ));
            s.setValue( "position", (pos() - mw->pos()));
        return QDialog::moveEvent( event );
    }

private slots:
    void cleverlyPosition() 
    {
        //Clever positioning
        QWidget* mw = findMainWindow();

        if( !mw ) return;

        resize( sizeHint() );

        AppSettings s;

        s.beginGroup( QString( metaObject()->className() ));
        QPoint offset = s.value( "position", QPoint( 40, 40 )).toPoint();
        s.endGroup();

        move( mw->pos() + offset);

        int screenNum = qApp->desktop()->screenNumber( mw );
        QRect screenRect = qApp->desktop()->availableGeometry( screenNum );
        if( !screenRect.contains( frameGeometry(), true)) {
            QRect diff;

            if( screenRect.contains( frameGeometry(), false )) {
                diff = frameGeometry().intersected( screenRect );
            } else {
                QPoint diffVector = frameGeometry().center() - screenRect.center();
                diff.setSize( QSize( diffVector.x(), diffVector.y())  );
                diff = diff.normalized();
            }
            int xDir = (diff.left() == screenRect.left() ? 1 : -1 );
            int yDir = (diff.top() == screenRect.top() ? 1 : -1 );
            QPoint adjust = QPoint((frameGeometry().width() - diff.width() ) * xDir, (frameGeometry().height() - diff.height()) * yDir);
            move( pos() + adjust );
        }
    }

private:
    bool m_firstShow;
    QWidget* findMainWindow() const
    {
        unicorn::MainWindow* mw = 0;
        foreach( QWidget* w, qApp->topLevelWidgets() ) {
            mw = qobject_cast<unicorn::MainWindow*>( w );
            if( mw ) break;
        }
        return mw;
    };
};

} //unicorn

#endif //UNICORN_DIALOG_H_
