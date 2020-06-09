#pragma once

#include "skin/skincontext.h"
#include "track/track.h"
#include "util/parented_ptr.h"
#include "widget/trackdroptarget.h"
#include "widget/wwidgetgroup.h"

class WTrackMenu;
class TrackCollectionManager;

class WTrackWidgetGroup : public WWidgetGroup, public TrackDropTarget {
    Q_OBJECT
  public:
    WTrackWidgetGroup(QWidget* pParent,
            UserSettingsPointer pConfig,
            TrackCollectionManager* pTrackCollectionManager,
            const char* group);
    ~WTrackWidgetGroup() override;
    void setup(const QDomNode& node, const SkinContext& context) override;

  signals:
    void trackDropped(QString fileName, QString group) override;
    void cloneDeck(QString source_group, QString target_group) override;

  public slots:
    void slotTrackLoaded(TrackPointer track);
    void slotLoadingTrack(TrackPointer pNewTrack, TrackPointer pOldTrack);

  protected:
    void paintEvent(QPaintEvent* pe) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

  private slots:
    void slotTrackChanged(TrackId);

  private:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void updateColor();

    const char* m_pGroup;
    UserSettingsPointer m_pConfig;
    TrackPointer m_pCurrentTrack;
    QColor m_trackColor;
    int m_trackColorAlpha;

    const parented_ptr<WTrackMenu> m_pTrackMenu;
};
