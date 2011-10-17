/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
**
****************************************************************************/

#include "qttools/core/log.h"

#include <QtCore/QList>
#include <QtCore/QLinkedList>
#include <QtCore/QMetaType>
#include <QtCore/QPair>

namespace {

QList<qttools::AbstractLogHandler*> globalLogHandlers;
typedef QPair<qttools::Log::MessageType, QString> PendingLogMessage;
QLinkedList<PendingLogMessage> globalPendingMessages;

void handleLogMessage(qttools::Log::MessageType msgType,
                      const QString& msg)
{
  if (globalLogHandlers.isEmpty())
    globalPendingMessages.append(qMakePair(msgType, msg));
  foreach (qttools::AbstractLogHandler* logHandler, globalLogHandlers)
    logHandler->handle(msgType, msg );
}

} // Anonymous namespace

namespace qttools {

/*! \class Log::Stream
 *  \brief Encapsulates a reference-counted QTextStream so a Log object can
 *         be quickly copied
 */

Log::Stream::Stream(MessageType mType)
  : msgType(mType),
    buffer(),
    ts(&buffer),
    refCount(1)
{
}

/*! \class Log
 *  \brief Provides an easy-to-use output stream for logging
 *
 *  This class is almost a clone of QDebug, but it's focused on logging.\n
 *  Also the way messages are handled is different as there can be multiple
 *  logging handlers attached (not just one).
 */

/*! \enum Log::MessageType
 *  \brief Types of logging message, with increasing severity
 */
/*! \var Log::MessageType Log::DebugMessage
 *  \brief
 */
/*! \var Log::MessageType Log::InfoMessage
 *  \brief
 */
/*! \var Log::MessageType Log::WarningMessage
 *  \brief
 */
/*! \var Log::MessageType Log::CriticalMessage
 *  \brief
 */
/*! \var Log::MessageType Log::FatalMessage
 *  \brief
 */

/*! \brief Construct a Log stream for messages of a special type
 */
Log::Log(MessageType msgType)
  : _stream(new Stream(msgType))
{
}

/*! \brief Construct a copy of the \p other Log stream
 */
Log::Log(const Log& other)
  : _stream(other._stream)
{
  ++(_stream->refCount);
}

/*! \brief Flush any pending data to be written and destroys the Log stream
 */
Log::~Log()
{
  --(_stream->refCount);
  if (_stream->refCount == 0) {
    ::handleLogMessage(_stream->msgType, _stream->buffer.toLocal8Bit().data());
    delete _stream;
  }
}

/*! \brief Write a space character to the Log stream and return a reference to
 *         the stream
 */
Log& Log::space()
{
  _stream->ts << ' ';
  return *this;
}

/*! \brief Write the boolean \p t to the log stream and return a reference to
 *         to the stream
 */
Log& Log::operator<<(bool t)
{
  _stream->ts << (t ? "true" : "false");
  return this->space();
}

Log& Log::operator<<(char t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(short t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(unsigned short t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(int t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(unsigned int t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(long t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(unsigned long t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(float t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(double t)
{
  _stream->ts << t;
  return this->space();
}

Log& Log::operator<<(const char* str)
{
  _stream->ts << str;
  return this->space();
}

Log& Log::operator<<(const QString& str)
{
  _stream->ts << str;
  return this->space();
}

void Log::registerMetaTypes()
{
  static bool alreadyRegistered = false;
  if (!alreadyRegistered) {
    qRegisterMetaType<MessageType>("Log::MessageType");
    qRegisterMetaType<MessageType>("qttools::Log::MessageType");
    alreadyRegistered = true;
  }
}

// --
// -- Related functions
// --

/*! \brief
 *  \relates Log
 */
Log debugLog()
{
  return Log(Log::DebugMessage);
}

/*! \brief
 *  \relates Log
 */
Log infoLog()
{
  return Log(Log::InfoMessage);
}

/*! \brief
 *  \relates Log
 */
Log warningLog()
{
  return Log(Log::WarningMessage);
}

/*! \brief
 *  \relates Log
 */
Log criticalLog()
{
  return Log(Log::CriticalMessage);
}

/*! \brief
 *  \relates Log
 */
Log fatalLog()
{
  return Log(Log::FatalMessage);
}


/*! \class AbstractLogHandler
 *  \brief Abstract base class of all logging message handlers
 */

AbstractLogHandler::AbstractLogHandler() :
  _autoDetach(false)
{
}

AbstractLogHandler::~AbstractLogHandler()
{
  if (_autoDetach)
    qttools::detachGlobalLogHandler(this);
}

/*! \fn void AbstractLogHandler::handle(Log::MessageType, const QString&)
 *  \brief Handle logging message \p msg of type \p msgType
 */

/*! \brief Set the log handler to auto-detach mode if \p b is true
 *
 *  If auto-detach is enabled then when the log handler is destroyed it gets
 *  automatically detached from the global list of log handlers.\n
 *  Auto-detach is disabled by default
 */
void AbstractLogHandler::setAutoDetach(bool b)
{
  _autoDetach = b;
}


/*! \brief Append \p handler to the global list of log handlers
 *
 *  If the internal list of log handlers is empty then all pending log
 *  messages are flushed to \p handler.
 *
 *  Note that log messages are recorded when there is no log handler attached.
 *  In this case a log message is considered as "pending".
 *
 *  \relates Log
 */
void attachGlobalLogHandler(AbstractLogHandler* handler)
{
  if (handler != 0) {
    if (::globalLogHandlers.isEmpty() && !::globalPendingMessages.isEmpty()) {
      foreach (const PendingLogMessage& iMsg, ::globalPendingMessages)
        handler->handle(iMsg.first, iMsg.second);
      ::globalPendingMessages.clear();
    }
    ::globalLogHandlers.append(handler);
  }
}

/*! \brief Remove \p handler from the global list of log handlers
 *  \relates Log
 */
void detachGlobalLogHandler(AbstractLogHandler* handler)
{
  if (handler != 0)
    ::globalLogHandlers.removeAll(handler);
}

/*! \class LogDispatcher
 *  \brief Dispatches a logging message as a Qt signal
 *
 */

LogDispatcher::LogDispatcher(QObject* parent) :
  QObject(parent)
{
}

/*! \brief Emits the signal log(msgType, msg)
 */
void LogDispatcher::handle(Log::MessageType msgType, const QString& msg)
{
  emit log(msgType, msg);
}

} // namespace qttools
