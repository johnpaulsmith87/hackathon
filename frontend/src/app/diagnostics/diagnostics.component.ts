import { Component, OnInit } from '@angular/core';
import { Observable, Subscription } from 'rxjs';
import { SerialService } from '../serial.service';

@Component({
  selector: 'app-diagnostics',
  templateUrl: './diagnostics.component.html',
  styleUrls: ['./diagnostics.component.css']
})
export class DiagnosticsComponent implements OnInit {


  constructor(private serialService: SerialService) { }

  ngOnInit() {
    this.serialService.status.subscribe(status => console.log(status))
  }

}
