/* Key binding functions */
static void togglegaps(const Arg *arg);
/* Layouts */
static void tile(Monitor *m);
/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);

/* Settings */
static int enablegaps = 1;

static void
togglegaps(const Arg *arg)
{
	enablegaps = !enablegaps;
	arrange(NULL);
}

static void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc )
{
	unsigned int n, oe = enablegaps, ie = enablegaps;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gappoh*oe; // outer horizontal gap
	*ov = m->gappov*oe; // outer vertical gap
	*ih = m->gappih*ie; // inner horizontal gap
	*iv = m->gappiv*ie; // inner vertical gap
	*nc = n;            // number of clients
}

/***
 * Layouts
 */

static void
tile(Monitor *m)
{
	unsigned int i, n, h, r, mw, my, ty;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? (m->ww - iv) * m->mfact: 0;
	else
		mw = m->ww - 2*ov;
	for (i = 0, my = ty = oh, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			r = MIN(n, m->nmaster) - i;
			h = (m->wh - my - oh - ih * (r - 1)) / r;
			resize(c, m->wx + ov, m->wy + my, mw - (2*c->bw) - oh, h - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			r = n - i;
			h = (m->wh - ty - oh - ih * (r - 1)) / r;
			resize(c, m->wx + mw + ov + (m->nmaster ? iv : 0) - oh, m->wy + ty, m->ww - mw - (2*c->bw) - 2*ov - (m->nmaster ? iv : 0) + oh, h - (2*c->bw), 0);
			ty += HEIGHT(c) + ih;
		}
}
