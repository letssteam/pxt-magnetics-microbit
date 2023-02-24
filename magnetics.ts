
//% groups='["Sending", "Receiving"]'
//% color=#00c3c3 weight=100 icon="\uf076"
namespace magnetics {
    
    class OnMessageEvent {
        public from: string;
        public cb: (data: string) => void;
        public lastData: string;

        public constructor(from: string, cb: (data: string) => void ){
            this.from = from;
            this.cb = cb;
            this.lastData = "";
        }
    };

    let onNewMessageEvents: OnMessageEvent[] = [];
    let isOnNewMessageConfigured: boolean = false;

    //% blockId=magnetics_startadvertising
    //% shim=magnetics::startAdvertising
    //% block="Start sending"
    //% weight=93
    //% group="Sending"
    export function startAdvertising(){
        return;
    }

    //% blockId=magnetics_stopadvertising
    //% shim=magnetics::stopAdvertising
    //% block="Stop sending"
    //% weight=92
    //% group="Sending"
    export function stopAdvertising(){
        return;
    }

    //% blockId=magnetics_setdevicename 
    //% shim=magnetics::setName
    //% block="Set device name to %name"
    //% group="Sending"
    export function setName(name: string){
        return;
    }

    //% blockId=magnetics_sendstring
    //% shim=magnetics::setData
    //% block="Send string %name"
    //% group="Sending"
    export function setData(str: string){
        return;
    }

    //% blockId=magnetics_setnumber
    //% block="Send number %name"
    //% group="Sending"
    export function sendNumber(value: number){
        setData("" + value);
    }

    //% blockId=magnetics_setvalue 
    //% block="Send value %name : %value"
    //% group="Sending"
    export function sendValue(name: string, value: number){
        setData( "" + (name.isEmpty() ? "" : name) + value );
    }

    //% blockId=magnetics_startScanning 
    //% shim=magnetics::startScanning
    //% block="Start receiving"
    //% weight=91
    //% group="Receiving"
    export function startScanning(){
        return;
    }

    //% blockId=magnetics_stopScanning 
    //% shim=magnetics::stopScanning
    //% block="Stop receiving"
    //% weight=90
    //% group="Receiving"
    export function stopScanning(){
        return;
    }

    //% blockId=magnetics_availableScanFor 
    //% shim=magnetics::availableScanFor
    //% block="is available data from %name"
    //% weight=80
    //% group="Receiving"
    export function availableScanFor(name: string) : boolean{
        return false;
    }

    //% blockId=magnetics_getDataFrom 
    //% shim=magnetics::getDataFrom
    //% block="read data from %name"
    //% weight=75
    //% group="Receiving"
    export function getDataFrom(name: string) : string{
        return "";
    }


    //% blockId=magnetics_onnewmessagefrom
    //% block="On new message from %from"
    //% draggableParameters=reporter
    //% weight=70
    //% group="Receiving"
    export function onNewMessageFrom(from: string, cb: (message: string) => void){

        if( !isOnNewMessageConfigured ){
            onNewMessage( onNewMessageHandler );
            isOnNewMessageConfigured = true;
        }

        onNewMessageEvents.push( new OnMessageEvent(from, cb) );
    }

    //% blockId=magnetics_onmessage
    //% shim=magnetics::onNewMessage
    export function onNewMessage( cb: Action){
        return;
    }

    //% blockId=magnetics_peekDataFrom 
    //% shim=magnetics::peekDataFrom
    export function peekDataFrom(name: string) : string{
        return "";
    }

    export function onNewMessageHandler() : void {
        onNewMessageEvents.forEach( (e) => {
            if( availableScanFor(e.from) ){
                let peek = peekDataFrom(e.from);

                if( peek != e.lastData ){
                    e.lastData = peek;
                    e.cb( peek );
                }
            }
        });
    
    }

}