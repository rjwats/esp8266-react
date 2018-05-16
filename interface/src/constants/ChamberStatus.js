import * as Highlight from '../constants/Highlight';

export const CHAMBER_STATUS_IDLE = 0;
export const CHAMBER_STATUS_HEATING = 1;
export const CHAMBER_STATUS_COOLING = 2;

export const chamberStatusHighlight = chamberStatus => {
  switch (chamberStatus.status){
    case CHAMBER_STATUS_HEATING:
      return Highlight.ERROR;
    case CHAMBER_STATUS_COOLING:
      return Highlight.INFO;
    case CHAMBER_STATUS_IDLE:
    default:
      return Highlight.IDLE;
  }
}

export const chamberStatus = chamberStatus => {
  switch (chamberStatus.status){
    case CHAMBER_STATUS_IDLE:
      return "Idle";
    case CHAMBER_STATUS_HEATING:
      return "Heating";
    case CHAMBER_STATUS_COOLING:
      return "Cooling";
    default:
      return "Unknown"
  }
}
