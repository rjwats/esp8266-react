import React from 'react';
import {withNotifier} from '../components/SnackbarNotification';

export const restComponent = (endpointUrl, FormComponent) => {

  return withNotifier(
    class extends React.Component {

      constructor(props) {
        super(props);

        this.setState = this.setState.bind(this);
        this.loadData = this.loadData.bind(this);
        this.saveData = this.saveData.bind(this);
        this.setData = this.setData.bind(this);
      }

      setData(data) {
        this.setState({
                 data:data,
                 fetched: true,
                 errorMessage:null
               });
      }

      loadData() {
        this.setState({
                 data:null,
                 fetched: false,
                 errorMessage:null
               });
        fetch(endpointUrl)
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => {this.setState({data: json, fetched:true})})
          .catch(error =>{
            this.props.raiseNotification("Problem fetching: " + error.message);
            this.setState({data: null, fetched:true, errorMessage:error.message});
          });
      }

      saveData(e) {
        this.setState({fetched: false});
        fetch(endpointUrl, {
          method: 'POST',
          body: JSON.stringify(this.statedata),
          headers: new Headers({
            'Content-Type': 'application/json'
          })
        })
        .then(response => {
          if (response.status === 200) {
            return response.json();
          }
          throw Error("Invalid status code: " + response.status);
        })
        .then(json => {
          this.props.raiseNotification("Changes successfully applied.");
          this.setState({data: json, fetched:true});
        }).catch(error => {
          this.props.raiseNotification("Problem saving: " + error.message);
          this.setState({data: null, fetched:true, errorMessage:error.message});
        });
      }

      valueChange = name => event => {
        const { data } = this.state;
        data[name] = event.target.value;
        this.setState({data});
      };

      checkboxChange = name => event => {
        const { data } = this.state;
        data[name] = event.target.checked;
        this.setState({data});
      }

      render() {
        return <FormComponent
                  valueChange={this.valueChange}
                  checkboxChange={this.checkboxChange}
                  setData={this.setData}
                  saveData={this.saveData}
                  loadData={this.loadData}
                  {...this.state}
                  {...this.props}
                />;
      }

    }
  );
}
