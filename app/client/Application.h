/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Max Howell, Jono Cole and Doug Mansell

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUDIOSCROBBLER_APPLICATION_H_
#define AUDIOSCROBBLER_APPLICATION_H_

#include <QPointer>
#include <QSystemTrayIcon>

#include <lastfm/global.h>
#include <lastfm/Track>
#include <lastfm/ws.h>

#include "lib/unicorn/UnicornApplication.h"

class AboutDialog;
class MainWindow;
class RadioWidget;
class QAction;
class ScrobbleInfoFetcher;
class Drawer;

#ifdef Q_WS_X11
    class IpodDeviceLinux;
#endif

#if defined(aApp)
#undef aApp
#endif
#define aApp (static_cast<audioscrobbler::Application*>(QCoreApplication::instance()))

namespace audioscrobbler
{
    
    /**
      * @brief Main application logic for the audioscrobbler app.
      *
      * This class contains the core components of the application
      * (ie Audioscrobbler, PlayerConnection etc), top-level gui widgets and the system tray.
      */
    class Application : public unicorn::Application
    {
        Q_OBJECT

        enum Argument
        {
            LastFmUrl,
            Pause, //toggles pause
            Skip,
            Exit,
            ArgUnknown
        };

        // we delete these so QPointers
        QPointer<QSystemTrayIcon> m_tray;
        QPointer<MainWindow> m_mw;

        Track m_currentTrack;
        Track m_trackToScrobble;

        void* m_raiseHotKeyId;

        AboutDialog* m_aboutDialog;
        
        QAction* m_submit_scrobbles_toggle;
        QAction* m_artist_action;
        QAction* m_title_action;
        QAction* m_love_action;
        QAction* m_tag_action;
        QAction* m_share_action;
        QAction* m_ban_action;
        QAction* m_play_action;
        QAction* m_skip_action;
        QAction* m_show_window_action;
        QAction* m_toggle_window_action;
        QAction* m_scrobble_ipod_action;
        QAction* m_visit_profile_action;
        QAction* m_faq_action;
        QAction* m_forums_action;
        QAction* m_about_action;
        QAction* m_prefs_action;
        
    public:
        Application(int& argc, char** argv);

        void init();
        
        QAction* loveAction() const{ return m_love_action; }
        QAction* tagAction() const{ return m_tag_action; }
        QAction* shareAction() const{ return m_share_action; }
        QAction* banAction() const{ return m_ban_action; }
        QAction* playAction() const{ return m_play_action; }
        QAction* skipAction() const{ return m_skip_action; }

        QSystemTrayIcon* tray() const { return m_tray.data(); }
        void setRaiseHotKey( Qt::KeyboardModifiers mods, int key );
        
    signals:
        void lovedStateChanged(bool loved);

        // re-route all the info fetchers singals
        void trackGotInfo(const XmlQuery& lfm);
        void albumGotInfo(const XmlQuery& lfm);
        void artistGotInfo(const XmlQuery& lfm);
        void artistGotEvents(const XmlQuery& lfm);
        void trackGotTopFans(const XmlQuery& lfm);
        void trackGotTags(const XmlQuery& lfm);

        void finished();
		
        void error( const QString& message );
        void status( const QString& message, const QString& id );

    public slots:
        void quit();
        void actuallyQuit();

        void changeLovedState(bool loved);
        void onBusLovedStateChanged(bool);

        void onTrackGotInfo(const XmlQuery& );
        void parseArguments( const QStringList& args );

    protected:
        virtual void initiateLogin()throw( StubbornUserException );

    private:
        static Argument argument( const QString& arg );

    private slots:
        void onTrayActivated(QSystemTrayIcon::ActivationReason);
        void onCorrected(QString correction);

        void onTagTriggered();
        void onShareTriggered();

        void onVisitProfileTriggered();
        void onFaqTriggered();
        void onForumsTriggered();
        void onAboutTriggered();
        void onPrefsTriggered();

        void showWindow();
        void toggleWindow();

        void onTrackStarted( const Track&, const Track& );
        void onTrackPaused( bool );

        void onTrackSpooled( const Track& , const Track& );

        void onMessageReceived(const QStringList& message);
		
		/** all webservices connect to this and emit in the case of bad errors that
	     * need to be handled at a higher level */
	    void onWsError( lastfm::ws::Error );
    };
}

#endif //AUDIOSCROBBER_APPLICATION_H_