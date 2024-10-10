let container = document.getElementsByClassName("btns_div")[0];

let output0_text = document.getElementById("output0");
let output1_text = document.getElementById("output1");
let output2_text = document.getElementById("output2");
let output3_text = document.getElementById("output3");

let input_field = document.getElementsByClassName("input")[0];

let cnf = document.getElementById("cnf");
let cpy = document.getElementById("cpy");
let inp = document.getElementById("inp");

let btns = [];

let output0 = 0;
let output1 = 0;
let output2 = 0;
let output3 = 0;
let mdown = 0;

let rows = 2 * 6;
let cols = 2 * 5;
let pix = rows * cols;

let curr_div;

for (let i = 0; i < pix; i++)
{
    if (i % cols == 0)
    {
        curr_div = document.createElement("div");
        curr_div.className = "button_div"
        container.append(curr_div);
    }
    let btn = document.createElement("button");
    btn.id = String(i);
    btn.style.backgroundColor = "antiquewhite";
    curr_div.appendChild(btn);
    btns.push(btn);
    btn.onmouseover = function ()
    {
        if (mdown == 1) {
            if (btn.style.backgroundColor == "antiquewhite") {
                btn.style.backgroundColor = "black";
                if (btn.id < pix / 4) {
                    output0 = output0 | (1 << (i));
                } else if (btn.id < pix / 2) {
                    output1 = output1 | (1 << (i-(pix/4)));
                } else if (btn.id < (pix / 4) * 3) {
                    output2 = output2 | (1 << (i-(pix/2)));
                } else {
                    output3 = output3 | (1 << (i-(pix / 4) * 3));
                }
            }
            else {
                btn.style.backgroundColor = "antiquewhite";
                if (btn.id < pix / 4) {
                    output0 = output0 & ~(1 << (i));
                } else if (btn.id < pix / 2) {
                    output1 = output1 & ~(1 << (i-(pix/4)));
                } else if (btn.id < (pix / 4) * 3) {
                    output2 = output2 & ~(1 << (i-(pix/2)));
                } else {
                    output3 = output3 & ~(1 << (i-(pix / 4) * 3));
                }
            }
            output0_text.innerText = "0x" + output0.toString(16).padStart(8, "0");
            output1_text.innerText = "0x" + output1.toString(16).padStart(8, "0");
            output2_text.innerText = "0x" + output2.toString(16).padStart(8, "0");
            output3_text.innerText = "0x" + output3.toString(16).padStart(8, "0");
        }
    }
    btn.onclick = function ()
    {
        if (btn.style.backgroundColor == "antiquewhite") {
            btn.style.backgroundColor = "black";
            if (btn.id < pix / 4)
            {
                output0 = output0 | (1 << (i));
            } else if(btn.id < pix / 2)
            {
                output1 = output1 | (1 << (i-(pix/4)));
            }else if (btn.id < (pix / 4) * 3)
            {
                output2 = output2 | (1 << (i-(pix/2)));
            } else
            {
                output3 = output3 | (1 << (i-(pix / 4) * 3));
            }
        }
        else {
            btn.style.backgroundColor = "antiquewhite";
            if (btn.id < pix / 4)
            {
                output0 = output0 & ~(1 << (i));
            } else if(btn.id < pix / 2)
            {
                output1 = output1 & ~(1 << (i-(pix/4)));
            } else if (btn.id < (pix / 4) * 3)
            {
                output2 = output2 & ~(1 << (i-(pix/2)));
            } else
            {
                output3 = output3 & ~(1 << (i-(pix / 4) * 3));
            }
        }
        output0_text.innerText = "0x" + output0.toString(16).padStart(8, "0");
        output1_text.innerText = "0x" + output1.toString(16).padStart(8, "0");
        output2_text.innerText = "0x" + output2.toString(16).padStart(8, "0");
        output3_text.innerText = "0x" + output3.toString(16).padStart(8, "0");
    }
}

cnf.onclick = function ()
{
    output0 = 0;
    output1 = 0;
    output2 = 0;
    output3 = 0;
    output0_text.innerText = "0x" + output0.toString(16).padStart(8, "0");
    output1_text.innerText = "0x" + output1.toString(16).padStart(8, "0");
    output2_text.innerText = "0x" + output2.toString(16).padStart(8, "0");
    output3_text.innerText = "0x" + output3.toString(16).padStart(8, "0");
    for (let i = 0; i < pix; i++)
    {
        btns[i].style.backgroundColor = "antiquewhite";
    }
}

cpy.onclick = function ()
{
    navigator.clipboard.writeText("{"+output0_text.innerText+","+output1_text.innerText+","+output2_text.innerText+","+output3_text.innerText+"};");
}

body.onmousedown = function ()
{
    mdown = 1;
}
body.onmouseup = function ()
{
    mdown = 0;
}

inp.onclick = function ()
{
    let text = input_field.value;
    text = text.replace("{", " ");
    text = text.replace("}", " ");
    text = text.replace(";", " ");
    text = text.trim();
    text = text.split(",");
    output0 = parseInt(text[0]);
    output1 = parseInt(text[1]);
    output2 = parseInt(text[2]);
    output3 = parseInt(text[3]);
    output0_text.innerText = "0x" + output0.toString(16).padStart(8, "0");
    output1_text.innerText = "0x" + output1.toString(16).padStart(8, "0");
    output2_text.innerText = "0x" + output2.toString(16).padStart(8, "0");
    output3_text.innerText = "0x" + output3.toString(16).padStart(8, "0");

    let outputs = [ output0, output1, output2, output3 ]        ;

    for (let i = 0; i < pix; i++)
    {
        let byIndexT = Math.floor(i / 30);
        if ((outputs[byIndexT] & (1 << (i % 30))) > 0)
        {
            btns[i].style.backgroundColor = "black";
        } else
        {
            btns[i].style.backgroundColor = "antiquewhite";
        }
    }
}