import './App.css';

import React from "react";


import { Nav, Navbar, ButtonToolbar, Button, Container } from "react-bootstrap"
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faPlayCircle, faSync, faCaretRight, faForward, faBackward, faCaretLeft } from '@fortawesome/free-solid-svg-icons'
import { Chart } from 'chart.js'
import axios from "axios"
import _ from 'lodash'

// Plugin for drawing vertical lines on the graph.
const verticalLinePlugin = {
    getLinePosition: function (chart, xval) {
        const left = chart.chartArea.left;
        const right = chart.chartArea.right;
        const xstart = chart.scales['x-axis-0'].start;
        const xend = chart.scales['x-axis-0'].end;
        return (xval - xstart) / (xend - xstart) * (right - left) + left;
    },

    // Renders black or grey line showing location of the current event.
    renderVerticalLine: function (chartInstance, pointIndex) {
        const scale = chartInstance.scales['y-axis-0'];
        const context = chartInstance.chart.ctx;

        if (typeof (pointIndex) === 'number') {
            // render vertical line
            const lineLeftOffset = this.getLinePosition(chartInstance, pointIndex);
            context.beginPath();
            context.strokeStyle = 'black';
            context.moveTo(lineLeftOffset, scale.top);
            context.lineTo(lineLeftOffset, scale.bottom);
            context.stroke();
        } else {
            const lineLeftOffset1 = this.getLinePosition(chartInstance, pointIndex[0]);
            const lineLeftOffset2 = this.getLinePosition(chartInstance, pointIndex[1]);
            context.fillStyle = 'lightgray';
            context.fillRect(lineLeftOffset1, scale.bottom, lineLeftOffset2 - lineLeftOffset1, scale.top - scale.bottom);
        }
    },

    // Renders the red line showing a warning
    renderVerticalLineRed: function (chartInstance, pointIndex) {
        const scale = chartInstance.scales['y-axis-0'];
        const context = chartInstance.chart.ctx;

        if (typeof (pointIndex) === 'number') {
            // render vertical line
            const lineLeftOffset = this.getLinePosition(chartInstance, pointIndex);
            context.beginPath();
            context.strokeStyle = 'red';
            context.moveTo(lineLeftOffset, scale.top);
            context.lineTo(lineLeftOffset, scale.bottom);
            context.stroke();
        }
    },

    beforeDatasetsDraw: function (chart, easing) {
        if (chart.config.lineAtIndex) {
            chart.config.lineAtIndex.forEach(pointIndex => this.renderVerticalLine(chart, pointIndex));
        }
        if (chart.config.warningAtIndex) {
            chart.config.warningAtIndex.forEach(pointIndex => this.renderVerticalLineRed(chart, pointIndex));
        }
    }
};

Chart.plugins.register(verticalLinePlugin);


class Process extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            // Whether to show program text
            show_process: true,
            
            // Whether to show graph
            show_graph: false,   

            // current lines
            cur_lines: Array()
        }
        if(props.onRef){//如果父组件传来该方法 则调用方法将子组件this指针传过去
            props.onRef(this,this.props.index)
        }  
    }
    
   
    // Convert the value of a variable to string for display.
    displayValue(val) {
        if (typeof (val) == 'number') {
            // Round numbers to at most three digits
            return String(Math.round(val.toFixed(3) * 1000) / 1000);
        }
        else if (typeof (val) == 'string') {
            // Display of strings
            return val;
        }
        else if (Array.isArray(val)) {
            // Display of lists
            return '[' + val.map((k) => this.displayValue(k)).join(',') + ']';
        }
        else {
            // Display of dictionary (structs)
            return '{' + (Object.keys(val).map((k) => k + ':' + this.displayValue(val[k]))).join(',') + '}';
        }
    }

    toggleShowText = (e) => {
        this.setState((state) => ({
            show_process: !state.show_process,
        }))
        this.props.parent.state.process_controllers[this.props.index].setState((state) => ({
            show_process: !state.show_process,
        }))
    }

    toggleShowGraph = (e) => {
        this.setState((state) => ({
            show_graph: !state.show_graph,
        }))
    }

    returninfo(){
        this.props.parent.getProcessname(this.props.name)   
    }
    returnProcessInfo = (e) => {
        this.returninfo()
        this.props.parent.forceUpdate()
    }

    render() {
        return (
            <div>
                
                {/* Program text, with highlight on current location */}
                <div class={this.props.name}>Process: {this.props.name}{'  '}
                    <a href="#" style={{ fontSize: 14 }} onClick={this.toggleShowText}>
                        {this.state.show_process ? "Hide Process" : "Show Process"}
                    </a>
                    {"       "}
                    <a href="#" style={{ fontSize: 14 }} onClick={this.returnProcessInfo}>
                        {"Show callstack"}
                    </a>
                </div>
                <div className="procedure">
                {
                    this.state.show_process ? (this.props.procedures.map((info, index) => {                       
                        return(
                            <div className="procedure">Procedure{index}:{info.name}
                            {
                                info.lines.map((str, line_no) => {
                                    if (this.props.callstack !== undefined) {
                                        var tag = 0;//tag=0,not in procedure
                                        if (this.props.callstack.procedure === null || this.props.callstack.procedure === undefined)
                                            tag = 0
                                        else 
                                            for (var ind = 0; ind < this.props.callstack.procedure.length; ind++)
                                                if (info.name === this.props.callstack.procedure[ind]){
                                                    tag = 1;
                                                    break;
                                                }
                                        if (tag !== 0) {   
                                            if (this.props.callstack['innerpos'][ind] !== undefined) {
                                                const pos = this.props.callstack['innerpos'][ind];
                                                var bg_start, bg_end;
                                                if (line_no === pos.start_x) {
                                                    bg_start = pos.start_y;
                                                } else if (line_no > pos.start_x) {
                                                    bg_start = 0;
                                                } else {
                                                    bg_start = str.length;
                                                }
                                                if (line_no === pos.end_x) {
                                                    bg_end = pos.end_y;
                                                } else if (line_no < pos.end_x) {
                                                    bg_end = str.length;
                                                } else {
                                                    bg_end = 0;
                                                }
                                                if (bg_start < bg_end) {
                                                    return (
                                                        <pre key={line_no}>
                                                            <span>{str.slice(0, bg_start)}</span>
                                                            <span className={this.props.npos ? "program-text-next-hl" : "program-text-hl"}>
                                                                {str.slice(bg_start, bg_end)}
                                                            </span>
                                                            <span>{str.slice(bg_end, str.length)}</span>
                                                        </pre>)
                                                }
                                            }
                                        }
                                    }
                                    return <pre key={line_no}>{str}</pre>
                                })
                            }
                        </div>)
                    })) : null
                }
                </div>
                <div className="program-text">
                    {
                        this.state.show_process ? (
                            <div>{   
                                this.props.lines.map((str, line_no) => {
                                        if (this.props.callstack !== undefined) {
                                            const pos = this.props.callstack['innerpos'][this.props.callstack['innerpos'].length-1];
                                            if (pos !== undefined) { 
                                                var bg_start, bg_end;
                                                if (line_no === pos.start_x) {
                                                    bg_start = pos.start_y;
                                                } else if (line_no > pos.start_x) {
                                                    bg_start = 0;
                                                } else {
                                                    bg_start = str.length;
                                                }
                                                if (line_no === pos.end_x) {
                                                    bg_end = pos.end_y;
                                                } else if (line_no < pos.end_x) {
                                                    bg_end = str.length;
                                                } else {
                                                    bg_end = 0;
                                                }
                                                if (bg_start < bg_end) {                                                  
                                                    return (
                                                        <pre key={line_no}>
                                                            <span>{str.slice(0, bg_start)}</span>
                                                            <span className={this.props.npos ? "program-text-next-hl" : "program-text-hl"}>
                                                                {str.slice(bg_start, bg_end)}
                                                        </span>
                                                            <span>{str.slice(bg_end, str.length)}</span>
                                                        </pre>)
                                                }
                                            }   
                                        }
                                        return <pre key={line_no}>{str}</pre>
                                    })
                                }
                            </div>
                        ) : null
                    }
                </div>

                {/* State of the program */}
                < pre className="program-state" >
                    <span>&nbsp;</span>
                    {this.props.statenum === undefined ? null:(
                    Object.keys(this.props.statemap[this.props.statenum]).map((key, index) => {
                        var val = this.props.statemap[this.props.statenum][key];
                        var str_val = this.displayValue(val);
                        return (<>
                            {index > 0 ? <><br /><span>&nbsp;</span></> : null}
                            <span key={index} style={{ marginLeft: "10px" }}>
                                <span style={{ color: 'indianred' }}> {key}: </span>
                                <span style={{ color: 'black' }}> {str_val} </span>
                            </span>
                        </>)
                    }))}
                    <span>&nbsp;&nbsp;</span>
                    <a href="#" onClick={this.toggleShowGraph}>
                        {this.state.show_graph ? "Hide graph" : "Show graph"}
                    </a>
                </pre >

                {/* Graph of time series */}
                {
                    (this.state.show_graph && this.props.time_series !== undefined) ?
                        <canvas onClick={this.getCurPosition}
                                id={'chart' + String(this.props.index)}
                                width="400" height="100"/> : null
                }
            </div >
        );
    }

    getCurPosition = (e) => {
        const map = this.canvas.getBoundingClientRect();
        const x = e.clientX - map.left;
        const y = e.clientY - map.top;
        if (x > this.chart.chartArea.left &&
            x < this.chart.chartArea.right &&
            y < this.chart.chartArea.bottom &&
            y > this.chart.chartArea.top)
        {    
            const ptx = x - this.chart.chartArea.left;
            this.props.onClick(e, ptx/(this.chart.chartArea.right-this.chart.chartArea.left)*this.chart.scales["x-axis-0"].end,this.props.name);
        }
        else
        {
            this.chart.handleEvent(e)
        }
    }
    
    componentDidUpdate() {
        const ts = this.props.time_series;
        if (!this.state.show_graph || ts === undefined || ts.length === 0) {
            return;
        }
        var series = {};
        const is_discrete = (ts[ts.length - 1].time === 0)
        for (let i = 0; i < ts.length; i++) {
            for (let k in ts[i].state) {
                if (typeof (ts[i].state[k]) === 'number') {
                    if (!(k in series)) {
                        series[k] = [];
                    }
                    if (is_discrete) {
                        series[k].push({ x: ts[i].event, y: ts[i].state[k] });
                    } else {
                        series[k].push({ x: ts[i].time, y: ts[i].state[k] });
                    }
                }
            }
        }
        var datasets = [];
        var colors = ['blue', 'red', 'green', 'yellow'];
        for (let k in series) {
            let color = datasets.length >= 4 ? 'black' : colors[datasets.length];
            datasets.push({
                label: k,
                lineTension: 0,
                backgroundColor: color,
                borderColor: color,
                borderWidth: 1,
                fill: false,
                pointRadius: 0,
                data: series[k]
            })
        }

        this.canvas = document.getElementById('chart' + String(this.props.index));

        const lineAtIndex = is_discrete ? this.props.hpos : (this.props.event_time.length>1?this.props.event_time[1]:this.props.event_time);
        if (typeof this.props.warning_at == 'undefined') { }
        else
            var warningAtIndex = this.props.warning_at[0];
        this.chart = new Chart(this.canvas, {
            type: 'line',
            data: {
                datasets: datasets
            },
            lineAtIndex: [lineAtIndex],
            warningAtIndex: [warningAtIndex],
            options: {
                animation: {
                    duration: 0
                },
                hover: {
                    animationDuration: 0
                },
                responsiveAnimationDuration: 0,
                scales: {
                    xAxes: [{
                        type: "linear",
                        display: true,
                        ticks: {
                            suggestedMin: 0,
                        }
                    }],
                    yAxes: [{
                        display: true,
                    }]
                },

            }
        })
       
        this.chart.update();
    }
}

class Events extends React.Component {
    render() {
        return (
            <div className="event-list">
                {this.props.events.map((event, index) => {
                    if (this.props.show_event_only && event.type === 'step') {
                        return null
                    } else {
                        return (
                            <pre key={index} title={"time: " + event.time} onClick={(e) => this.props.onClick(e, index)}>
                                <span className={index === this.props.current_index ? "event-list-hl" : ""}>
                                    {event.id + ' ' + event.str}
                                </span>
                            </pre>
                        )
                    }
                })}
            </div>
        )
    }
}
class Processes extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            // Whether to show program text
            show_process: true,

        }
        if(props.onRef){//如果父组件传来该方法 则调用方法将子组件this指针传过去
            props.onRef(this,this.props.index)
        }  
    }

    gettarget(){
        this.target = this.props.parent.state.process_texts[this.props.index]
    }

    name_on_click = (e) => {
        if(document.getElementsByClassName(this.props.name)[0])
            document.getElementsByClassName(this.props.name)[0].scrollIntoView();
    }

    show_on_click = (e) => {
        this.gettarget()
        this.target.toggleShowText()
    }

    show_callstack_on_click = (e) =>{
        this.gettarget()
        this.target.returninfo()
        this.props.parent.forceUpdate()
    }

    render(){
        return(
            <div className="processes-list">
                <div>
                    <a href="#" style={{fontSize: 14,text_decoration: "underline", color: "black"}} onClick={this.name_on_click}>
                        {this.props.name}
                    </a>
                    {":"}
                    <span>&nbsp;&nbsp;</span>
                    <a href="#" style={{ fontSize: 14 }} onClick={this.show_on_click}>
                            {this.state.show_process ? "Hide Process" : "Show Process"}
                    </a>
                    <span>&nbsp;&nbsp;</span>
                    <a href="#" style={{ fontSize: 14 }} onClick={this.show_callstack_on_click}>
                        {"Show callstack"}
                    </a>
                </div>
            </div>
        )
    }
}

class Callstack extends React.Component {
    render() {
        return (
            <div className="callstack-list">
                <div>Callstacks:
                {this.props.hcsp_info ?(
                    this.props.hcsp_info.procedures.map((info, index) => {    
                        if (this.props.callstack !== undefined) {
                            var tag = 0;//tag=0,not in procedure
                            if (this.props.callstack.procedure === null || this.props.callstack.procedure === undefined)
                                tag = 0
                            else 
                                for (var ind = 0; ind < this.props.callstack.procedure.length; ind++)
                                    if (info.name === this.props.callstack.procedure[ind]){
                                        tag = 1;
                                        break;
                                    }
                        }
                        if(tag === 1){        
                            return(  
                                this.props.callstack['innerpos'][ind] ? (
                                <pre>   
                                    {info.name}:line{this.props.callstack['innerpos'][ind].start_x}:{info.lines[this.props.callstack['innerpos'][ind].start_x]}
                                </pre>
                                ):null
                            )
                        }
                    })
                ):null}
                </div>
                <div>
                    {this.props.hcsp_info ? (
                        this.props.callstack ?(
                            this.props.callstack['innerpos'][this.props.callstack['innerpos'].length-1] ?(
                            <pre>
                                {this.props.hcsp_info.name}:line{this.props.callstack['innerpos'][this.props.callstack['innerpos'].length-1].start_x}:{this.props.hcsp_info.lines[this.props.callstack['innerpos'][this.props.callstack['innerpos'].length-1].start_x]}
                            </pre>
                            ):null
                        ):null
                ):null}
                </div>
            </div>
        )
    }
}


class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            // Name of the currently open file
            hcspFileName: "",

            // Info about the HCSP program
            hcsp_info: [],

            // Currently computed history. Each entry represents one
            // state in the execution.
            // Each entry contains the position and state of the program
            // at each step.
            history: [],
            
            // State hash map
            statemap: undefined,

            // Time series information
            time_series: [],

            // Current position
            history_pos: 0,

            // Warning message from simulation
            sim_warning: undefined,

            // Whether there is a file loaded.
            file_loaded: false,

            // Error from server.
            error: undefined,

            // Whether a query is in progress.
            querying: false,

            // Maximum number of events for the query.
            num_steps: 200,

            // Maximum number of events shown for the query.
            num_show: 200,

            // Showing starting from event number.
            show_starting: 0,

            // Whether to show events only.
            show_event_only: false,

            // Warnings from checks of channel mismatches.
            warnings: [],   
            
            // Tell callstack part current process
            cur_process: undefined,

            // Tell callstack part current callstack
            cur_callstack: undefined,
            
            // this pivots of all process-text
            process_texts: [],

            // this pivots of all process-controller
            process_controllers: [], 
        };
        this.reader = new FileReader();
        this.fileSelector = undefined;
        this.picclicked=false;
    }

    getProcessname = (msg) =>{
        for (var i=0;i<this.state.hcsp_info.length;i++){
            if (this.state.hcsp_info[i].name === msg){
                if (this.state.cur_process != i){
                    this.state.cur_process = i
                }
                break;
            }
        }
    }

    
    onRef_texts = (ref,index) => {
        this.state.process_texts[index] = ref
    }

    onRef_controllers = (ref,index) => {
        this.state.process_controllers[index] = ref
    }

    handleChange = (e) => {
        const name = e.target.name;

        this.setState({
            [name]: Number(e.target.value)
        })
    }

    handleFiles = () => {
        this.reader.onloadend = async () => {
            this.setState({
                querying: true
            })
            const response = await axios.post("/parse_hcsp", {
                text: this.reader.result,
            })
            if ('warnings' in response.data) {
                this.setState({
                    warnings: response.data.warnings
                })
            }
            if ('error' in response.data) {
                this.setState({
                    error: response.data.error,
                    hcsp_info: [],
                    hcspFileName: this.fileSelector.files[0].name,
                    file_loaded: true,
                    history: [],
                    time_series: [],
                    history_pos: 0,
                    querying: false
                })
            } else {
                this.setState({
                    error: undefined,
                    hcsp_info: response.data.hcsp_info,
                    hcspFileName: this.fileSelector.files[0].name,
                    file_loaded: true,
                    history: [],
                    time_series: [],
                    history_pos: 0,
                    querying: false
                });
            }
        };
        this.reader.readAsText(this.fileSelector.files[0]);
    };


    buildFileSelector = () => {
        const fileSelector = document.createElement('input');
        fileSelector.type = "file";
        fileSelector.onchange = this.handleFiles;
        return fileSelector;
    };

    handleFileSelect = (e) => {
        e.preventDefault();
        this.fileSelector.value = "";
        this.fileSelector.click();
    };

    run = async (e) => {
        e.preventDefault();
        this.setState({
            querying: true
        })
        const response = await axios.post("/run_hcsp", {
            hcsp_info: this.state.hcsp_info,
            num_steps: this.state.num_steps,
            num_show: this.state.num_show,
            show_event_only: this.state.show_event_only
        })
        if ('error' in response.data) {
            this.setState({
                error: response.data.error,
                history: [],
                history_pos: 0,
                time_series: [],
                querying: false
            })
        } else {
            this.setState({
                error: undefined,
                history: response.data.trace,
                history_pos: 0,
                statemap: response.data.statemap,
                time_series: response.data.time_series,
                sim_warning: response.data.warning,
                querying: false
            })
            if ('warning' in response.data) {
                var time = Math.round(response.data.warning[0].toFixed(5) * 100000) / 100000;
                var str_time = 'At time ' + String(time);
                this.setState({
                    history_pos: response.data.trace.length - 1,
                    warnings: [str_time, response.data.warning[1]]
                })
                document.getElementById('event').scrollTop = (this.state.history_pos) * 21;
            } else {
                document.getElementById('event').scrollTop = 0;
            }
        }
    };

    nextEvent = (e) => {
        if (!this.state.show_event_only) {
            document.getElementById('event').scrollTop += 21;
        }
        this.setState((state) => ({
            history_pos: state.history_pos + 1,
        }))
        
    };

    prevEvent = (e) => {
        if (!this.state.show_event_only) {
            document.getElementById('event').scrollTop -= 21;
        }
        this.setState((state) => ({
            history_pos: state.history_pos - 1,
        }))
        
    };

    eventOnClick = (e, i) => {
        this.setState({
            history_pos: i,
        })
        
    }

    nextStep = (e) => {
        var hpos = this.state.history_pos + 1;
        while (hpos < this.state.history.length - 1 && this.state.history[hpos].type === 'step') {
            hpos += 1;
        }
        document.getElementById('event').scrollTop += (hpos - this.state.history_pos) * 21;
        this.setState({
            history_pos: hpos
        })
        
    };

    prevStep = (e) => {
        var hpos = this.state.history_pos - 1;
        while (hpos > 0 && this.state.history[hpos].type === 'step') {
            hpos -= 1;
        }
        document.getElementById('event').scrollTop -= (this.state.history_pos - hpos) * 21;
        this.setState({
            history_pos: hpos
        })
        
    };

    setStateAsync = (state) => {
        return new Promise((resolve) => {
            this.setState(state, resolve);
        });
    };

    eventLine = () => {
        if (this.state.history.length === 0) {
            return "No data"
        } else {
            const history = this.state.history;
            var res;
            if (history[history.length - 1].type === 'deadlock') {
                res = String(history.length - 1) + " events."
            } else {
                res = String(history.length - 1) + "+ events."
            }
            return res
        }
    };

    toggleShowEvent = () => {
        this.setState((state) => ({
            show_event_only: !state.show_event_only
        }))
    }

    jumptocurstep = () => {
        if(document.getElementsByClassName('program-text-next-hl')[0])
            document.getElementsByClassName('program-text-next-hl')[0].scrollIntoView();
    }
    picOnClick = (e, i,hcsp_name) => {
        var index = 0;
        const ts = this.state.time_series[hcsp_name];
        if (ts === undefined || ts.length === 0) {
            var is_discrete = true 
        }else{
            var is_discrete = (ts[ts.length - 1].time === 0)
        }
        if (is_discrete) {
            while (index<i){
                //console.log(this.state.history[index].time)
                index++;
            }
        } else {
            while (this.state.history[index]['time']<i){
                //console.log(this.state.history[index].time)
                index++;
                if (this.state.history[index] === undefined)
                    break;
            }
        }
        if (index > this.state.num_show)
            index = this.state.num_show;
        this.setState({
            history_pos: index,    
        })

        this.picclicked=true;
    }

    showDetails = async () => {
        var cur_pos = this.state.history_pos;
        var start_pos = this.state.history[cur_pos];
        var start_event = {
            id: start_pos.id,
            infos: start_pos.infos,
            time: start_pos.time
        }
        this.setState({
            querying: true
        })
        const response = await axios.post("/run_hcsp", {
            hcsp_info: this.state.hcsp_info,
            num_steps: 999,
            num_show: 999,
            start_event: start_event,
            show_event_only: this.state.show_event_only
        })
        console.log(response);
        if ('error' in response.data) {
            this.setState({
                error: response.data.error,
                history: [],
                history_pos: 0,
                time_series: [],
                querying: false
            })
        } else {
            this.setState({
                error: undefined,
                history: this.state.history.slice(0, cur_pos-1).concat(response.data.trace,
                         this.state.history.slice(cur_pos-1, this.state.history.length)),
                history_pos: cur_pos,
                sim_warning: response.data.warning,
                querying: false
            })
        }
    }

    render() {
        const left = this.state.error !== undefined ?
            <pre className="error-message">
                Error: {this.state.error}
            </pre>
            : (
                <Container className="left">
                   
                    {this.state.warnings.map((warning, index) => {
                        return <div key={index} style={{ color: 'red' }}>{warning}</div>
                    })}
                    {this.state.hcsp_info.map((info, index) => {
                        const hcsp_name = info.name;
                        if ('parallel' in info) {
                            return <div key={index}>Process {hcsp_name} ::= {info.parallel.join(' || ')}</div>
                        }
                        else if (this.state.history.length === 0) {
                            // No data is available
                            return <Process key={index} index={index} lines={info.lines} procedures={info.procedures} onRef={this.onRef_texts}
                                    name={hcsp_name} callstack={undefined} statenum={statenum} statemap={this.state.statemap}
                                    time_series={time_series} event_time={event_time} hpos={undefined} npos={undefined} 
                                    warning_at={this.state.sim_warning} onClick={this.picOnClick} parent={this}/> 
                        } else {
                            const hpos = this.state.history_pos;
                            const event = this.state.history[hpos];
                            var callstack, statenum;
                            if (typeof (event.infos[hcsp_name]) === 'number') {
                                var prev_id = event.infos[hcsp_name];
                                var prev_info = this.state.history[prev_id].infos[hcsp_name];
                                callstack = prev_info.callstack;
                                statenum = prev_info.statenum;
                            } else {
                                callstack = event.infos[hcsp_name].callstack;
                                statenum = event.infos[hcsp_name].statenum;
                            }
                            var event_time;
                            if (event.type !== 'delay') {
                                event_time = event.time;
                            } else {
                                event_time = [event.time, event.time + event.delay_time];
                            }
                            var time_series = this.state.time_series[hcsp_name];
                            for (var i = 0; i < callstack['innerpos'].length; i++){
                                var pos = callstack['innerpos'][i]
                                if (pos === 'end') {
                                    // End of data set   
                                    return <Process key={index} index={index} lines={info.lines} procedures={info.procedures} onRef={this.onRef_texts}
                                            name={hcsp_name} callstack={undefined} statenum={statenum} statemap={this.state.statemap}
                                            time_series={time_series} event_time={event_time} hpos={hpos} npos={undefined} 
                                            warning_at={this.state.sim_warning} onClick={this.picOnClick} parent={this}/>     
                                }
                            }    
                            var callstack_temp = _.cloneDeep(callstack);
                            var npos = false;
                            if (hpos < this.state.history.length) {
                                callstack_temp['innerpos'] = [];
                                callstack_temp['procedure'] = [];
                                if ('ori_pos' in this.state.history[hpos] && hcsp_name in this.state.history[hpos].ori_pos)
                                    var callstack_length = this.state.history[hpos].ori_pos[hcsp_name].innerpos.length
                                else
                                    var callstack_length = this.state.history[hpos].infos[hcsp_name].callstack.innerpos.length      
                                for (i = 0 ; i < callstack_length ; i++){
                                    if ('ori_pos' in this.state.history[hpos] && hcsp_name in this.state.history[hpos].ori_pos)
                                    {
                                        if(this.state.history[hpos].ori_pos[hcsp_name].procedure === null)
                                            var proc =null
                                        else
                                            var proc = this.state.history[hpos].ori_pos[hcsp_name].procedure[i]    
                                    }   
                                    else
                                    {
                                        if(this.state.history[hpos].infos[hcsp_name].callstack.procedure === null)
                                            var proc = null
                                        else
                                            var proc = this.state.history[hpos].infos[hcsp_name].callstack.procedure[i];
                                    }
                                    if (proc === null){
                                        if ('ori_pos' in this.state.history[hpos] && hcsp_name in this.state.history[hpos].ori_pos)
                                        {    
                                            npos = true;
                                            callstack_temp['innerpos'][i] = info.mapping[this.state.history[hpos].ori_pos[hcsp_name].innerpos[i]];
                                        }
                                        else
                                            callstack_temp['innerpos'][i] = info.mapping[this.state.history[hpos].infos[hcsp_name].callstack.innerpos[i]];
                                    }
                                    else{
                                        for (var ind=0; index < info.procedures.length; ind++){    
                                            if(info.procedures[ind].name === proc)
                                                        break;
                                        }
                                        if ('ori_pos' in this.state.history[hpos] && hcsp_name in this.state.history[hpos].ori_pos)
                                        {
                                            npos = true;
                                            callstack_temp['innerpos'][i] = info.procedures[ind].mapping[this.state.history[hpos].ori_pos[hcsp_name].innerpos[i]];    
                                        }
                                        else
                                            callstack_temp['innerpos'][i] = info.procedures[ind].mapping[this.state.history[hpos].infos[hcsp_name].callstack.innerpos[i]];       
                                    }
                                }
                                if (this.state.history[hpos].infos[hcsp_name].callstack.procedure === null){
                                    callstack_temp['procedure'] = null
                                }
                                else{
                                    for (i = 0; i < this.state.history[hpos].infos[hcsp_name].callstack.innerpos.length; i++){
                                        if ('ori_pos' in this.state.history[hpos] && hcsp_name in this.state.history[hpos].ori_pos)
                                            callstack_temp['procedure'][i] = this.state.history[hpos].ori_pos[hcsp_name].procedure[i];
                                        else
                                            callstack_temp['procedure'][i] = this.state.history[hpos].infos[hcsp_name].callstack.procedure[i];
                                    }
                                }     
                            }
                            if (this.state.cur_process == index){
                                this.state.cur_callstack = callstack_temp
                            }
                            return <Process key={index} index={index} lines={info.lines} procedures={info.procedures} onRef={this.onRef_texts}
                                    name={hcsp_name} callstack={callstack_temp} statenum={statenum} statemap={this.state.statemap}
                                    time_series={time_series} event_time={event_time} hpos={hpos} npos={npos} 
                                    warning_at={this.state.sim_warning} onClick={this.picOnClick} parent={this}/>    
                        }
                    })}
                </Container>
            );
        const event = (
            <Container id="event" className="event">
                <Events events={this.state.history} current_index={this.state.history_pos}
                    onClick={this.eventOnClick} show_event_only={this.state.show_event_only} />
            </Container>
        );
        const callstack=(
            <Container id="callstack" className="callstack">
                <Callstack hcsp_info ={this.state.hcsp_info[this.state.cur_process]} callstack={this.state.cur_callstack}/>
            </Container>
        );
        const processes=(
            <Container id="processes" className="processes">
                {
                    this.state.hcsp_info.map((info, index) => {
                        const hcsp_name = info.name;
                        if ('parallel' in info) {
                            return <div key={index}>Process {hcsp_name} ::= {info.parallel.join(' || ')}</div>
                        }
                        else if (this.state.history.length === 0) {
                            // No data is available
                            return <Processes key={index} index={index} name={hcsp_name} parent={this} onRef={this.onRef_controllers}/> 
                        } else {
                            return <Processes key={index} index={index} name={hcsp_name} parent={this} onRef={this.onRef_controllers}/> 
                        }
                    })
                }
            </Container>
        )
        return (
            <div>
                <Navbar bg="light" variant="light" id="navbar">
                    <Navbar.Brand href="#">HCSP Simulator</Navbar.Brand>
                    <Nav className="mr-auto">
                        <Button variant={"primary"} onClick={this.handleFileSelect}>Read HCSP File</Button>
                        <span style={{ marginLeft: '20px', fontSize: 'x-large' }}>{this.state.hcspFileName}</span>
                        <label htmlFor="num_steps" className="menu-label">Number of steps:</label>
                        <input type="text" id="num_steps" name="num_steps" style={{ width: '70px' }} value={this.state.num_steps} onChange={this.handleChange} />
                        <label htmlFor="num_show" className="menu-label">Showing </label>
                        <input type="text" id="num_show" name="num_show" value={this.state.num_show} onChange={this.handleChange} />
                        <label htmlFor="show_starting" className="menu-label">starting from </label>
                        <input type="text" id="show_starting" name="show_starting" style={{ width: '70px' }} value={this.state.show_starting} onChange={this.handleChange} />
                    </Nav>
                </Navbar>

                <div className="toolbar">
                    <ButtonToolbar>
                        <Button variant="success" title={"run"} onClick={this.run}
                            disabled={this.state.querying || !this.state.file_loaded || this.state.error !== undefined}>
                            <FontAwesomeIcon icon={faPlayCircle} size="lg" />
                        </Button>

                        <Button variant="secondary" title={"refresh"} onClick={this.handleFiles}
                            disabled={this.state.querying || !this.state.file_loaded}>
                            <FontAwesomeIcon icon={faSync} size="lg" />
                        </Button>

                        <Button variant="secondary" title={"forward"} onClick={this.nextEvent}
                            disabled={this.state.querying || this.state.history.length === 0 || this.state.history_pos === this.state.history.length - 1}>
                            <FontAwesomeIcon icon={faCaretRight} size="lg" />
                        </Button>

                        <Button variant="secondary" title={"backward"} onClick={this.prevEvent}
                            disabled={this.state.querying || this.state.history.length === 0 || this.state.history_pos === 0}>
                            <FontAwesomeIcon icon={faCaretLeft} size="lg" />
                        </Button>

                        <Button variant="secondary" title={"step forward"} onClick={this.nextStep}
                            disabled={this.state.querying || this.state.history.length === 0 || this.state.history_pos === this.state.history.length - 1}>
                            <FontAwesomeIcon icon={faForward} size="lg" />
                        </Button>

                        <Button variant="secondary" title={"step backward"} onClick={this.prevStep}
                            disabled={this.state.querying || this.state.history.length === 0 || this.state.history_pos === 0}>
                            <FontAwesomeIcon icon={faBackward} size="lg" />
                        </Button>

                        <span style={{ marginLeft: '20px', fontSize: 'large', marginTop: "auto", marginBottom: "auto" }}>
                            {this.eventLine()}
                        </span>

                        <a href="#" style={{ marginLeft: '10px', marginRight: '10px', marginTop: 'auto', marginBottom: 'auto' }}
                            onClick={this.jumptocurstep}>
                            {'Cur step'}
                        </a>

                        <a href="#" style={{ marginLeft: 'auto', marginRight: '10px', marginTop: 'auto', marginBottom: 'auto' }}
                            onClick={this.toggleShowEvent}>
                            {this.state.show_event_only ? 'Show all steps' : 'Show events only'}
                        </a>
                        <a href="#" style={{ marginLeft: '10px', marginRight: '10px', marginTop: 'auto', marginBottom: 'auto' }}
                            onClick={this.showDetails}>
                            {'Show details'}
                        </a>

                    </ButtonToolbar>
                </div>

                <hr className="headline"/>
                {left}
                {event}
                {callstack}
                {processes}
            </div>
        );
    }
    componentDidUpdate() {
        if(document.getElementsByClassName('event-list-hl')[0] && this.picclicked===true)
            document.getElementsByClassName('event-list-hl')[0].scrollIntoView();
        this.picclicked=false;
    }
    componentDidMount() {
        this.fileSelector = this.buildFileSelector();    
    }
}

export default App;
