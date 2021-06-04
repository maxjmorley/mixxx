#include "skin/qml/qmlplayerproxy.h"

#include "mixer/basetrackplayer.h"

// FIXME: Unfornately, clazy can stumble over these preprocessor macros and
// show false-positive warnings. Therefore, the clazy-incorrect-emit warnings
// are disabled in this source file.
//
// clazy:excludeall=incorrect-emit

#define PROPERTY_IMPL(TYPE, NAME, GETTER, SETTER)    \
    TYPE QmlPlayerProxy::GETTER() const {            \
        const TrackPointer pTrack = m_pCurrentTrack; \
        if (pTrack == nullptr) {                     \
            return TYPE();                           \
        }                                            \
        return pTrack->GETTER();                     \
    }                                                \
                                                     \
    void QmlPlayerProxy::SETTER(const TYPE& value) { \
        const TrackPointer pTrack = m_pCurrentTrack; \
        if (pTrack != nullptr) {                     \
            pTrack->SETTER(value);                   \
            emit NAME##Changed();                    \
        }                                            \
    }

namespace mixxx {
namespace skin {
namespace qml {

QmlPlayerProxy::QmlPlayerProxy(BaseTrackPlayer* pTrackPlayer, QObject* parent = nullptr)
        : QObject(parent), m_pTrackPlayer(pTrackPlayer) {
    connect(m_pTrackPlayer,
            &BaseTrackPlayer::loadingTrack,
            this,
            &QmlPlayerProxy::slotLoadingTrack);
    connect(m_pTrackPlayer,
            &BaseTrackPlayer::newTrackLoaded,
            this,
            &QmlPlayerProxy::slotTrackLoaded);
    connect(m_pTrackPlayer,
            &BaseTrackPlayer::playerEmpty,
            this,
            &QmlPlayerProxy::trackUnloaded);
    connect(this, &QmlPlayerProxy::trackChanged, this, &QmlPlayerProxy::slotTrackChanged);
}

void QmlPlayerProxy::slotTrackLoaded(TrackPointer pTrack) {
    m_pCurrentTrack = pTrack;
    if (pTrack != nullptr) {
        connect(pTrack.get(),
                &Track::changed,
                this,
                &QmlPlayerProxy::trackChanged);
        connect(pTrack.get(),
                &Track::keyChanged,
                this,
                &QmlPlayerProxy::keyTextChanged);
        connect(pTrack.get(),
                &Track::colorUpdated,
                this,
                &QmlPlayerProxy::colorChanged);
    }
    emit trackChanged();
    emit trackLoaded();
}

void QmlPlayerProxy::slotLoadingTrack(TrackPointer pNewTrack, TrackPointer pOldTrack) {
    Q_UNUSED(pNewTrack);
    Q_UNUSED(pOldTrack);
    const TrackPointer pTrack = m_pCurrentTrack;
    if (pTrack != nullptr) {
        disconnect(pTrack.get(), nullptr, this, nullptr);
    }
    m_pCurrentTrack.reset();
    emit trackChanged();
    emit trackLoading();
}

void QmlPlayerProxy::slotTrackChanged() {
    emit artistChanged();
    emit titleChanged();
    emit albumChanged();
    emit albumArtistChanged();
    emit genreChanged();
    emit composerChanged();
    emit groupingChanged();
    emit yearChanged();
    emit trackNumberChanged();
    emit trackTotalChanged();
    emit commentChanged();
    emit keyTextChanged();
    emit colorChanged();
}

PROPERTY_IMPL(QString, artist, getArtist, setArtist)
PROPERTY_IMPL(QString, title, getTitle, setTitle)
PROPERTY_IMPL(QString, album, getAlbum, setAlbum)
PROPERTY_IMPL(QString, albumArtist, getAlbumArtist, setAlbumArtist)
PROPERTY_IMPL(QString, genre, getGenre, setGenre)
PROPERTY_IMPL(QString, composer, getComposer, setComposer)
PROPERTY_IMPL(QString, grouping, getGrouping, setGrouping)
PROPERTY_IMPL(QString, year, getYear, setYear)
PROPERTY_IMPL(QString, trackNumber, getTrackNumber, setTrackNumber)
PROPERTY_IMPL(QString, trackTotal, getTrackTotal, setTrackTotal)
PROPERTY_IMPL(QString, comment, getComment, setComment)
PROPERTY_IMPL(QString, keyText, getKeyText, setKeyText)

QColor QmlPlayerProxy::getColor() const {
    const TrackPointer pTrack = m_pCurrentTrack;
    if (pTrack == nullptr) {
        return QColor();
    }
    return RgbColor::toQColor(pTrack->getColor());
}

void QmlPlayerProxy::setColor(const QColor& value) {
    const TrackPointer pTrack = m_pTrackPlayer->getLoadedTrack();
    if (pTrack != nullptr) {
        std::optional<RgbColor> color = RgbColor::fromQColor(value);
        pTrack->setColor(color);
        emit colorChanged();
    }
}

} // namespace qml
} // namespace skin
} // namespace mixxx
