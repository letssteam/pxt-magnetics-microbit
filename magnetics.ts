//% color=#00c3c3 weight=100 icon="\uf076"
namespace magnetics {
    
    //% blockId=magnetics_stopadvertising 
    //% shim=magnetics::startAdvertising
    //% block="Start sending"
    export function startAdvertising(){
        return;
    }

    //% blockId=magnetics_startadvertising 
    //% shim=magnetics::stopAdvertising
    //% block="Stop sending"
    export function stopAdvertising(){
        return;
    }

    //% blockId=magnetics_setdevicename 
    //% shim=magnetics::setName
    //% block="Set device name to %name"
    export function setName(name: string){
        return;
    }

    //% blockId=magnetics_sendstring
    //% shim=magnetics::setData
    //% block="Send string %name"
    export function setData(str: string){
        return;
    }

    //% blockId=magnetics_setnumber
    //% block="Send number %name"
    export function sendNumber(value: number){
        setData("" + value);
    }

    //% blockId=magnetics_setvalue 
    //% block="Send value %name : %value"
    export function sendValue(name: string, value: number){
        setData( "" + (name.isEmpty() ? "" : name) + value );
    }
}