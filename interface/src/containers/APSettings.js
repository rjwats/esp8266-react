import React, { Component } from 'react';

import { AP_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import APSettingsForm from '../forms/APSettingsForm';

class APSettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { fetched, errorMessage, data, saveData, loadData, handleValueChange } = this.props;
    return (
      <SectionContent title="AP Settings">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <APSettingsForm
              apSettings={data}
              onSubmit={saveData}
              onReset={loadData}
              handleValueChange={handleValueChange}
            />
          }
        />
      </SectionContent>
    )
  }

}

export default restComponent(AP_SETTINGS_ENDPOINT, APSettings);
