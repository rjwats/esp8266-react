import React, { Component } from "react";

import { RestControllerProps, RestFormLoader, SectionContent, restController } from "../components";
import { ChamberSettings, CHAMBER_SETTINGS_ENDPOINT, ChamberStatus, CHAMBER_STATUS_ENDPOINT } from "./types";
import ChamberSettingsForm from "./ChamberSettingsForm";

interface ChamberSettingsControllerProps extends RestControllerProps<ChamberSettings> {
  chamberStatus: ChamberStatus;
  loadChamberStatus: () => void;
}

class ChamberSettingsController extends Component<ChamberSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { chamberStatus, loadChamberStatus, ...rest } = this.props;
    return (
      <RestFormLoader
        {...rest}
        render={formProps => <ChamberSettingsForm
          {...formProps}
          chamberStatus={chamberStatus}
          loadData={() => {
            loadChamberStatus();
            formProps.loadData();
          }} />
        }
      />
    );
  }

}

const ChamberSettingsRestController = restController(CHAMBER_SETTINGS_ENDPOINT, ChamberSettingsController);

type ChamberStatusControllerProps = RestControllerProps<ChamberStatus>;

class ChamberSettingsControllerWithStatus extends Component<ChamberStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Chamber Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <ChamberSettingsRestController
            chamberStatus={formProps.data}
            loadChamberStatus={formProps.loadData}
          />}
        />
      </SectionContent>
    );
  }

}

export default restController(CHAMBER_STATUS_ENDPOINT, ChamberSettingsControllerWithStatus);
