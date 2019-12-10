// https://studio-kikoro.appspot.com/blog/20150415_mlp2.html
//

onload = function() {
    // プログラム表示と実行。
    document.getElementById('program1').innerText = program1;
    program1();
}

function program1() {

    var N = 50;          // 学習データ数。
    var ETA = 0.3;       // 学習率η（イータ）。
    // var NUM_TRAIN = 2000; // 学習回数。
    var NUM_TRAIN = 5; // 学習回数。
    var NUM_IN = 1 + 1;  // 入力層のユニット数。（バイアス含む）
    var NUM = [
        1 + 3,           // 隠れ層1のユニット数。（バイアス含む）
        1 + 3,           // 隠れ層2のユニット数。（バイアス含む）
        1 + 3,           // 隠れ層3のユニット数。（バイアス含む）
        1                // 出力層のユニット数。
    ];
    var L = NUM.length;  // 隠れ～出力の層数。

    /**
     * テストプログラムのmain()。
     *
     * 変数名凡例。
     * n         : 学習データのindex。
     * t         : 正解(教師)データ。
     * x,z1,z2,y : 入力,隠れ1,隠れ2,出力層の値。
     * i,j1,j2,k : 入力,隠れ1,隠れ2,出力層のユニットindex。
     * w1,w2,w3  : 入力-隠れ1,隠れ1-隠れ2,隠れ2-出力層の重み。
     */
    (function main() {
        document.getElementById('result1').innerText = '';

        var str_msg = [];

        // 学習データ（入力、正解）と検証用データの作成。
        var xlist = []; // xlist[N][NUM_INPUT] 入力。（数値x）
        var qlist = []; // qlist[N][NUM_INPUT] 検証用の未知入力。
        var tlist = []; // tlist[N][NUM_OUTPUT] 正解。（数値f(x)）
        var fx = document.getElementById('fx1').checked ? 1 :
                 document.getElementById('fx2').checked ? 2 :
                 document.getElementById('fx3').checked ? 3 : 4;
        for (var n = 0; n < N; ++n) {
            var x = (n / N) * 2 - 1;                         // x:-1～1
            // var x = (n / N) * 4 - 2;                         // x:-1～1
            xlist[n] = [1, x];                               // 1=バイアス, x
            qlist[n] = [1, x + Math.random() / N];           // 1=バイアス, x + noise
            if (fx == 1) tlist[n] = [x * x];                 // f(x) = x^2
            if (fx == 2) tlist[n] = [Math.abs(x)];           // f(x) = abs(x)
            if (fx == 3) tlist[n] = [Math.sin(Math.PI * x)]; // f(x) = sin(PIx)
            if (fx == 4) tlist[n] = [Math.sin(2 * Math.PI * x)]; // f(x) = sin(2PIx)
        }

        // 隠れ～出力の重みを乱数で初期化。w = [[ユニット数][前層ユニット数],..]
        var w = [];
        for (var l = 0; l < L; ++l) {
            w[l] = arand(NUM[l], (l == 0) ? NUM_IN : NUM[l - 1]);
        }
        logcat('学習前の誤差 e=' + calcError(L, NUM, xlist, tlist, w) + '\n');

        // 学習と認識を繰り返す。
        (function trainAndRecognize(loop) {

            str_msg[0] = '';
            // 学習データで学習。
            for (var n = 0; n < N; ++n) {
                train(L, NUM, xlist[n], tlist[n], w, str_msg);
            }

            // 検証データで認識。
            var zylist = [];
            for (var l = 0; l < L; ++l) {
                zylist[l] = [];
            }
            for (var n = 0; n < N; ++n) {
                var val = recognize(L, NUM, qlist[n], w);
                for (var l = 0; l < L; ++l) {
                    zylist[l][n] = val[l];
                }
            }

            // 結果をグラフに表示。
            // var gctx = initGraph(-1, 1, -1, 1);
            var gctx = initGraph(-2, 2, -4, 4);
            plot(gctx, xlist, tlist, 0, 'dot', 'red'); // 正解データ：赤点。
            plot(gctx, qlist, zylist[L - 1], 0, 'dot', 'blue'); // 検証結果：青点。
            var colors = ['gray', 'aqua', 'green', 'orange', 'navy']; // 隠れ層1～：色線。
            /* for (var l = 0; l < L - 1; ++l) {
                for (var j = 0; j < NUM[l]; ++j) {
                    plot(gctx, qlist, zylist[l], j, 'line', colors[l]);
                }
            }*/
            logcat('学習' + loop + '回目の誤差 e=' + calcError(L, NUM, xlist, tlist, w) + '\n' + str_msg[0] + '\n');
            // logcat('学習' + loop + '回目の誤差 e=' + calcError(L, NUM, xlist, tlist, w) + '\n');

            // 表示を反映するためsetTimeout()でyieldする。
            if (loop < NUM_TRAIN) {
                setTimeout(function() {trainAndRecognize(loop + 1);}, 1);
            }
        })(0);
    })();

    /**
     * 学習。（重みの更新）
     * @param L 層数。
     * @param NUM[] ユニット数。
     * @param x[] 入力値。
     * @param t[] 正解値。
     * @param w[[][],[][],..] 重み。
     * @return なし。
     */
    function train(L, NUM, x, t, w, str_msg) {

        // (1)順伝播で出力を計算。
        var val = recognize(L, NUM, x, w);

        // (2)逆伝播で誤差を計算。δ1=y-t, δ2=f'(z)Σw3･δ1, δ3=f'(z)Σw2･δ2, ..
        var dlist = [];
        for (var l = 0; l < L; ++l) { // 誤差δを初期化。
            dlist[l] = azero(NUM[l]);
        }
        str_msg[0] = str_msg[0] + '==== Back Propagating...\n'
        for (var l = L - 1; 0 <= l; --l) { // 誤差δを計算。
            str_msg[0] = str_msg[0] + '  Layer:' + l + '\n';
            if (l == L - 1) { // 出力層。
                var d = dlist[l];
                var y = val[l];
                for (var k = 0; k < NUM[l]; ++k) { // 本層ユニット数。
                    d[k] = y[k] - t[k];
                    str_msg[0] = str_msg[0] + '     d' + l + '[' + k + '] = y[' + k + '] - t[' + k + ']\n';
                    // str_msg[0] = str_msg[0] + '     d' + l + '[' + k + '] = y[' + k + '](' + y[k] + ') - t[' + k + '](' + t[k] + ')\n';
                }
            } else { // 隠れ層。
                var weight = w[l + 1];
                var d1 = dlist[l + 1];
                var d2 = dlist[l];
                var zy = val[l];
                var ll = l + 1;
                for (var j = 0; j < NUM[l]; ++j) { // 本層ユニット数。
                    var temp = 0;
                    str_msg[0] = str_msg[0] + '     d' + l + '[' + j + '] = ';
                    for (var k = 0; k < NUM[l + 1]; ++k) { // 次層ユニット数。
                        temp += weight[k][j] * d1[k];
                        str_msg[0] = str_msg[0] + 'w' + ll + '[' + k + '][' + j + ']*d' + ll + '[' + k + '] + ';
                        // str_msg[0] = str_msg[0] + 'w[' + k + '][' + j + '](' + weight[k][j] + ')*d' + ll + '[' + k + '](' + d1[k] + ') + ';
                    }
                    str_msg[0] = str_msg[0] + ':: And sigmoid : o' + l + '[' + j + ']*(1 - o' + l + '[' + j + '])*that\n';
                    // str_msg[0] = str_msg[0] + ':: And sigmoid : o' + l + '[' + j + '](' + zy[j] + ')*(1 - o' + l + '[' + j + '](' + zy[j] + '))*that(' + temp + ')\n';
                    d2[j] = zy[j] * (1 - zy[j]) * temp; // sigmoid
                }
            }
        }

        // (3)重みの更新。w1=w1-ETA･δ1･x, w2=w2-ETA･δ2･z1, w3=w3-ETA･δ3･z2, ..
        for (var l = 0; l < L; ++l) {
            var weight = w[l];
            var d = dlist[l];
            var xz = (l == 0) ? x : val[l - 1];
            str_msg[0] += '   === update layer:' + l + ' ===\n';
            for (var j = 0; j < NUM[l]; ++j) { // 本層ユニット数。
                for (var i = 0; i < xz.length; ++i) { // 前層ユニット数。
                    str_msg[0] += '       w' + l + '[' + j + '][' + i + '] -= ETA*d' + l + '[' + j + ']*last_input[' + i + ']\n';
                    // str_msg[0] += '       w' + l + '[' + j + '][' + i + '] -= ETA*d' + l + '[' + j + '](' + d[j] + ')*last_input[' + i + '](' + xz[i] + ')\n';
                    weight[j][i] -= ETA * d[j] * xz[i];
                }
                str_msg[0] += '\n';
            }
        }
    }

    /**
     * 認識。（値の算出）
     * @param L 層数。
     * @param NUM[] ユニット数。
     * @param x[] 入力値
     * @param w[[][],[][],..] 重み
     * @return [[],[],..] 隠れ,出力値
     */
    function recognize(L, NUM, x, w) {

        // 隠れ,出力値を初期化。
        var zy = [];
        for (var l  = 0; l < L; ++l) {
            zy[l] = azero(NUM[l]);
        }

        // 隠れ,出力値を計算。z1=f(Σw1･x), z2=f(Σw2･z1), ～ y=Σw3･z2
        for (var l = 0; l < L; ++l) {
            recogNn((l == 0) ? x : zy[l - 1], zy[l], w[l], l < L - 1);
        }
        return zy;

        // NNによる出力値計算。
        function recogNn(x, y, w, isHidden) {
            for (var j = 0; j < y.length; ++j) {
                var a = 0;
                for (var i = 0; i < x.length; ++i) {
                    a += w[j][i] * x[i];
                }
                if (isHidden) {
                    y[j] = 1 / (1 + Math.exp(-a)); // sigmoid
                } else {
                    y[j] = a;
                }
            }
            /* バイアス項があると収束しづらいので除去。
            if (isHidden) {
                y[0] = 1; // 1=バイアス。
            }
            */
        }
    }

    /**
     * 二乗誤差の和を計算。e=1/2Σ(y-t)^2
     * @param L 層数。
     * @param NUM[] ユニット数。
     */
    function calcError(L, NUM, xlist, tlist, w) { 
        var error = 0;
        for (var n = 0; n < N; ++n) {
            var val = recognize(L, NUM, xlist[n], w);
            var y = val.pop();
            for (var k = 0; k < y.length; ++k) {
                error += 0.5 * (y[k] - tlist[n][k]) * (y[k] - tlist[n][k]);
            }
        }
        return error;
    }

    /**
     * サブルーチン。
     */
    // 値0の配列生成。
    function azero(len) {
        var dst = [];
        for (var i = 0; i < len; ++i) {
            dst[i] = 0;
        }
        return dst;
    }

    // 乱数値(-1～1)の配列配列を生成。
    function arand(N, I) {
        var dst = [];
        for (var n = 0; n < N; ++n) {
            dst[n] = [];
            for (var i = 0; i < I; ++i) {
                dst[n][i] = Math.random() * 2 - 1;
            }
        }
        return dst;
    }

    // テキスト出力。
    function logcat(str) {
        document.getElementById('result1').innerText =
            str + document.getElementById('result1').innerText;
    }

    // グラフの初期化。
    function initGraph(xmin, xmax, ymin, ymax) {
        var gctx = {xmin:xmin, xmax:xmax, ymin:ymin, ymax:ymax}; // 表示範囲を設定。
        gctx.canvas = document.getElementById('canvas1'); // canvasとctxを準備する。
        gctx.canvas.width = gctx.canvas.offsetWidth; // 論理サイズを×１倍に設定。
        gctx.canvas.height = gctx.canvas.offsetHeight;
        gctx.ctx = gctx.canvas.getContext('2d');
        return gctx;
    }

    // グラフ描画。
    function plot(gctx, xlist, ylist, yidx, type, color) {
        gctx.ctx.save();
        gctx.ctx.fillStyle = color;
        gctx.ctx.strokeStyle = color;
        for (var i = 0; i < xlist.length; ++i) {
            var pos = mapPos(xlist[i][1], ylist[i][yidx]);
            if (type == 'dot') {
                gctx.ctx.beginPath();
                gctx.ctx.arc(pos.x, pos.y, 2, 0, Math.PI * 2, true);
                gctx.ctx.fill();
            } else if (type == 'line' && i < xlist.length - 1) {
                var pos2 = mapPos(xlist[i + 1][1], ylist[i + 1][yidx]);
                gctx.ctx.beginPath();
                gctx.ctx.moveTo(pos.x, pos.y);
                gctx.ctx.lineTo(pos2.x, pos2.y);
                gctx.ctx.stroke();
            }
        }
        gctx.ctx.restore();

        // 論理座標→canvas座標。
        function mapPos(x, y) {
            x = (x - gctx.xmin) / (gctx.xmax - gctx.xmin) * gctx.canvas.width;
            y = (1 - (y - gctx.ymin) / (gctx.ymax - gctx.ymin)) * gctx.canvas.height;
            return {x:x, y:y};
        }
    }
}

