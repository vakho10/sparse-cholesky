import { Component, OnInit } from '@angular/core';
import { ConfigService } from './services/config.service';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {

  result: Result[] = [];

  constructor(private config: ConfigService, private http: HttpClient) { }

  ngOnInit(): void {

    // Load config
    this.config.getConfig().subscribe((conf: Configuration) => {
      // Load results
      console.log(conf.resultsJsonPath)
      this.http.get(conf.resultsJsonPath).subscribe((res: Result[]) => {
        this.result = res;
        console.log(res);
      }, (error) => console.log(error))
    });
  }
}