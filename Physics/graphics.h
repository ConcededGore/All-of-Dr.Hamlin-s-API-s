#ifndef GROBS_H
#define GROBS_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <sstream>
#include <random>

// graphical objects

class QPainter;
class QPointF;
class QPaintDevice;

class Window;
class Widget;
class QPixmap;
class QMediaPlayer;
class QSoundEffect;
class QObject;
class QWidget;

enum class Key
{
    Left  = 17,
    Right = 18,
    Up    = 19,
    Down  = 20,
    Shift = 29,
    Ctrl  = 30,
    Alt   = 31
};

enum class EvtType
{
    MousePress,
    MouseRelease,
    MouseMove,
    KeyPress,
    KeyRelease,
};

enum class ModKey
{
    Shift = 1 << 0,
    Alt   = 1 << 1,
    Ctrl  = 1 << 2
};

namespace mssm
{
    class Color
    {
    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a{255};
    public:
        constexpr Color()  : r(0), g(0), b(0), a(255) {}
        constexpr Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)  : r(_r), g(_g), b(_b), a(_a) {}
    };

    constexpr Color TRANSPARENT(0,0,0,0);
    constexpr Color WHITE(255,255,255);
    constexpr Color BLACK(0,0,0);
    constexpr Color RED(255,0,0);
    constexpr Color GREEN(0,255,0);
    constexpr Color BLUE(0,0,255);
    constexpr Color YELLOW(255,255,0);
    constexpr Color PURPLE(255,0,255);
    constexpr Color CYAN(0,255,255);

    class Point
    {
    public:
        double x;
        double y;
    };


    class Event
    {
    public:
        EvtType evtType;
        int     x;
        int     y;
        ModKey  mods;
        int     arg;
    };

    class Grob;


    class Image
    {
    private:
        std::shared_ptr<QPixmap> pixmap;
    public:
        Image() {}
        Image(const std::string& filename);
        friend class Graphics;
    };

    class SoundInternal
    {
    public:
        std::string                   filename;
        std::shared_ptr<QSoundEffect> sound;
    public:
        SoundInternal(const std::string& filename);
    private:
        bool play(QObject* parent);  // returns true when first creating QSoundEffect
        int  status();
        void release();
        friend class Graphics;
    };

    class Sound
    {
    public:
        std::shared_ptr<SoundInternal> sound;
    };

    class Graphics
    {
    private:
        std::random_device randDevice;
        std::mt19937 mersenneTwister;
        std::function<void (Graphics&)> mainFunc;
        std::mutex glock;
        std::condition_variable cv;
        std::vector<std::unique_ptr<Grob>> queued;
        std::vector<std::unique_ptr<Grob>> active;

        std::vector<Sound>                 sounds;
        std::vector<Sound>                 soundCache;

        std::string                        musicFile;
        std::unique_ptr<QMediaPlayer>      musicPlayer;

        std::vector<Event> _events;
        std::vector<Event> _cachedEvents;
        std::string title;
        bool        closeOnExit{false};
        bool        closed{false};
        bool        finished{false};
        bool        isDrawn{false};
        bool        cleared{false};
        std::chrono::milliseconds::rep start_time;
        int         _width{100};
        int         _height{100};
        mssm::Color background;
        std::vector<bool> isPressed;
        std::vector<bool> wasPressed;
        std::string stringOutput;
        std::function<std::string()> getInputText;
        Point       _mousePos; // mouse pos at time of last screen repaint
    public:
        Graphics(std::string title, std::function<void (Graphics&)> mainFunc, int width = 1000, int height = 500);
       ~Graphics();

        void handleEvent(int x, int y, EvtType evtType, ModKey mods, int arg);
    public:
        std::istream in;
        std::ostream out;

        int    width()  { return _width; }
        int    height() { return _height; }

        Point  mousePos() { return _mousePos; }
        void   mousePos(int& x, int& y) { x = _mousePos.x; y = _mousePos.y; }

        void   setBackground(Color c) { background = c; }
        void   line(double x1, double y1, double x2, double y2, Color c = WHITE);
        void   ellipse(double x, double y, double w, double h, Color c = WHITE, Color f = TRANSPARENT);
        void   arc(double x, double y, double w, double h, double a, double alen, Color c = WHITE, Color f = TRANSPARENT);
        void   chord(double x, double y, double w, double h, double a, double alen, Color c = WHITE, Color f = TRANSPARENT);
        void   pie(double x, double y, double w, double h, double a, double alen, Color c = WHITE, Color f = TRANSPARENT);
        void   polygon(std::vector<Point> pts, Color border, Color fill);
        void   polyline(std::vector<Point> pts, Color color);
        void   text(double x, double y, double size, const std::string& str, Color textColor = WHITE);
        void   point(double x, double y, Color c);
        void   points(std::vector<Point> pts, Color c);

        void   image(double x, double y, const Image& image);

        Sound  createSound(const std::string& filename);
        void   play(Sound sound);
        void   music(const std::string& filename);

        void   clear();
        bool   draw(int delay = 0);

        void   setCloseOnExit(bool close = true) { closeOnExit = close; }
        bool   isClosed();
        void   waitUntilClosed();

        std::chrono::milliseconds::rep time();

        bool   isKeyPressed(char c) { return isPressed[c]; }
        bool   isKeyPressed(Key k);

        const  std::vector<Event>& events();

        int    randomInt(int minVal, int maxVal);
        double randomDouble(double minVal, double maxVal);

        std::string currentPath(const std::string& file = "");
        std::string programName();

        //std::string findFile(const std::string& filename);

    private:
        void draw(QWidget *pd, QPainter *painter, int width, int height, int elapsed);
        std::string getTitle() { return title; }
        void setClosed();
        void callMainFunc(Window *window);
        void addGrob(Grob* ptr);

        std::string getOutputText();
        bool appendOutputText(const std::string& txt);
        void setInputTextFunc(std::function<std::string()> func) { getInputText = func; }


        void setMousePos(int x, int y);

        friend void helper(Graphics* g, Window *window);
        friend class ::Widget;
        friend class ::Window;
    };
}

#endif // GROBS_H
