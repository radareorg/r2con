Reversing an AVR based ESC slides given in RadareCon2016.

This slide deck is based on the fantastic [reveal.js framework](https://github.com/hakimel/reveal.js).

## Quickstart

```
git clone https://github.com/brainstorm/radarecon2016
npm install
npm start
```

## PDF version of this talk

The builtin reveal.js PDF converter does not work properly, therefore I would suggest using [decktape](https://github.com/astefanutti/decktape):

```
$ docker run --rm --net=host -v `pwd`:/slides astefanutti/decktape http://<ABOVE_NODEJS_INSTANCE_IP>:8000 slides.pdf
```

## Video of the talk

[![Reversing an AVR-based ESC](http://img.youtube.com/vi/wVTXrmOyqwQ/0.jpg)](http://www.youtube.com/watch?v=wVTXrmOyqwQ)
