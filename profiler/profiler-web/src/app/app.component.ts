import { Component, OnInit } from '@angular/core';
import { ConfigService } from './config.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {

  constructor(private config: ConfigService) {
  }

  // Load configuration
  ngOnInit(): void {
    this.config.getConfig().subscribe((data: Configuration) => {
      console.log(data);
    });
  }
}
