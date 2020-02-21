import React, { Component } from "react";

import { RestControllerProps, RestFormLoader, SectionContent, restController } from "../../components";

import { CHAMBER_STATUS_ENDPOINT, ChamberStatus } from "./types";
import ChamberStatusForm from "./ChamberStatusForm";

type ChamberStatusControllerProps = RestControllerProps<ChamberStatus>;

class ChamberStatusController extends Component<ChamberStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Chamber Status" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <ChamberStatusForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(CHAMBER_STATUS_ENDPOINT, ChamberStatusController);
