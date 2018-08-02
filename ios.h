/** @brief iostream helpers.
 */

#ifndef IOS_H
#define IOS_H

#include <ios>
#include <streambuf>

template<class S> class ostream_capture
{
    S&                 stream_;
    std::streambuf*    old_buffer_;
    std::ostringstream redirect_;

  public:
    ostream_capture(S& stream) : stream_(stream), old_buffer_(stream.rdbuf())
    {
        stream_.rdbuf(redirect_.rdbuf());
    }
    ~ostream_capture() { stream_.rdbuf(old_buffer_); }

    auto str() { return redirect_.str(); }
};

#endif /* IOS_H */
