#pragma once

class BaseMap {
public:
    BaseMap(double lower, double upper, double speed);
    double get();
    operator double();
    operator float();

protected:
    double lower;
    double upper;
    double speed;

    double val;

    virtual void recompute();
    static double getSeconds();
};

// Define the classes. I use a macro to reduce work.
#ifndef DEFINE_MAP_CLASS

#define DEFINE_MAP_CLASS(x)                                                                        \
    x:                                                                                             \
public                                                                                             \
    BaseMap {                                                                                      \
        void recompute() override;                                                                 \
                                                                                                   \
    public:                                                                                        \
        x(double lower, double upper, double speed = 1.0);                                         \
    }

class DEFINE_MAP_CLASS(SinMap);
class DEFINE_MAP_CLASS(PingPongMap);
class DEFINE_MAP_CLASS(BouncyMap);

#undef DEFINE_MAP_CLASS

#endif // DEFINE_MAP_CLASS
